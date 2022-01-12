#include <dcore/Util/LoaderUtil.hpp>
#include <dcore/Data/Adapters/JsonAdapter.hpp>
#include <dcore/Data/FileInput.hpp>
#include <dcore/Core/Type.hpp>
#include <dcore/Core/Log.hpp>
#include <dcore/Uni.hpp>

#include <fstream>
#include <vector>
#include <string>
#include <set>

namespace dcore::util
{
	namespace
	{
		enum AccessorComponentType
		{
			ACCESSOR_SIGNED_BYTE    = 5120,
			ACCESSOR_UNSIGNED_BYTE  = 5121,
			ACCESSOR_SIGNED_SHORT   = 5122,
			ACCESSOR_UNSIGNED_SHORT = 5123,
			ACCESSOR_UNSIGNED_INT   = 5125,
			ACCESSOR_FLOAT          = 5126,
		};

		enum AccessorType
		{
			ACCESSOR_SCALAR,
			ACCESSOR_VEC2,
			ACCESSOR_VEC3,
			ACCESSOR_VEC4,
			ACCESSOR_MAT2,
			ACCESSOR_MAT3,
			ACCESSOR_MAT4,
		};

		AccessorType accessorTypeFromString(const std::string &str)
		{
			if(str == "SCALAR") return ACCESSOR_SCALAR;
			if(str == "VEC2") return ACCESSOR_VEC2;
			if(str == "VEC3") return ACCESSOR_VEC3;
			if(str == "VEC4") return ACCESSOR_VEC4;
			if(str == "MAT2") return ACCESSOR_MAT2;
			if(str == "MAT3") return ACCESSOR_MAT3;
			if(str == "MAT4") return ACCESSOR_MAT4;
			return static_cast<AccessorType>(-1);
		}

		struct BufferFile
		{
			dstd::USize Size = 0;
			std::string Path = "";
		};

		struct Buffer
		{
			BufferFile *File = nullptr;
			bool Allocated   = false;
			byte *Data       = nullptr;
		};

		struct BufferView
		{
			Buffer *Buffer         = nullptr;
			dstd::USize ByteOffset = 0;
			dstd::USize ByteLength = 0;
		};

		struct Accessor
		{
			BufferView *View                    = nullptr;
			AccessorComponentType ComponentType = (AccessorComponentType)0;
			AccessorType Type                   = (AccessorType)0;
			dstd::USize Count                   = 0;
			dstd::USize ByteOffset              = 0;
		};

		struct Material
		{
			std::string Name = "";
		};

		struct Primitive
		{
			Accessor *Position = nullptr;
			Accessor *Normal   = nullptr;
			Accessor *TexCoord = nullptr;
			Accessor *Indices  = nullptr;
			Material *Material = nullptr;
		};

		void fillBuffer(Buffer *buffer)
		{
			std::ifstream f {buffer->File->Path, std::ios::binary};
			f.read(reinterpret_cast<char *>(buffer->Data), buffer->File->Size);
			f.close();
		}

		dstd::USize getAccessorComponentTypeSize(AccessorComponentType t)
		{
			switch(t)
			{
			case ACCESSOR_SIGNED_BYTE: return 1;
			case ACCESSOR_UNSIGNED_BYTE: return 1;
			case ACCESSOR_SIGNED_SHORT: return 2;
			case ACCESSOR_UNSIGNED_SHORT: return 2;
			case ACCESSOR_UNSIGNED_INT: return 4;
			case ACCESSOR_FLOAT: return 4;
			default: return 0;
			}
		}

		dstd::USize getAccessorTypeSize(AccessorType t)
		{
			switch(t)
			{
			case ACCESSOR_SCALAR: return 1;
			case ACCESSOR_VEC2: return 2;
			case ACCESSOR_VEC3: return 3;
			case ACCESSOR_VEC4: return 4;
			case ACCESSOR_MAT2: return 4;
			case ACCESSOR_MAT3: return 9;
			case ACCESSOR_MAT4: return 16;
			default: return 0;
			}
		}

		dstd::USize getIndexSize(Accessor *a)
		{
			return getAccessorComponentTypeSize(a->ComponentType) * getAccessorTypeSize(a->Type);
		}

		dstd::USize computeActualOffset(Accessor *a, dstd::USize index)
		{
			auto offset = a->View->ByteOffset + a->ByteOffset;
			return offset + index * getIndexSize(a);
		}

		template<typename T>
		T convertBytesAt(Accessor *a, dstd::USize offset)
		{
			T value     = {};
			byte *bytes = reinterpret_cast<byte *>(&value);
			auto size   = std::min(getAccessorComponentTypeSize(a->ComponentType), (dstd::USize)sizeof(T));

			// Copying the integer bytes manually because size is known only at runtime
			// Maybe should make ifs for all known sizes for it to be faster, idk, just
			// need it to be done for now.
			for(byte j = 0; j < size; ++j) bytes[j] = a->View->Buffer->Data[offset + j] << (j * 8);

			return value;
		}

		template<typename T, int C>
		glm::vec<C, T> convertElementAt(Accessor *a, dstd::USize offset, T defaultValue = T {})
		{
			glm::vec<C, T> value(defaultValue);
			auto size = std::min(getAccessorTypeSize(a->Type), (dstd::USize)C);

			for(byte i = 0; i < size; ++i)
				value[i] = convertBytesAt<T>(a, offset + getAccessorComponentTypeSize(a->ComponentType) * i);

			return value;
		}
	} // namespace

	bool LoaderUtil::LoadModel(graphics::ModelData &d, const std::string &folder, const std::string &gltfPath)
	{
		static bool shouldLog = true;
#define DLOG(MSG) \
	if(shouldLog) DCORE_LOG_INFO << MSG
		// NOTE, TODO: Becasue of how Blender exports glTFs, we assume that there is not byteStride and each component
		//             has its own accessor/bufferView

		data::FileInput fi(folder, gltfPath);
		data::adapters::JsonInputAdapter jia;
		fi.SetAdapter(&jia);
		fi.Read();
		auto &gltf = fi.Get();

		std::vector<BufferFile> bufferFiles;
		bufferFiles.reserve(gltf["buffers"].size());

		DLOG("reading buffers");
		for(const auto &buffer : gltf["buffers"])
		{
			bufferFiles.push_back(
			    BufferFile {buffer["byteLength"].get<dstd::USize>(), folder + "/" + buffer["uri"].get<std::string>()});
		}

		// Overall, we ignore scenes, nodes and meshes and only include "primitives" in output.

		std::vector<Buffer> buffers;
		std::vector<BufferView> bufferViews;
		std::vector<Accessor> accessors;
		std::vector<Material> materials;
		std::vector<Primitive> primitives;

		std::set<Buffer *> usedBuffers;
		std::set<BufferView *> usedBufferViews;
		std::set<int> usedAccessors;

		std::vector<dstd::USize> nodes;
		DLOG("reading scenes");
		for(const auto &scene : gltf["scenes"])
		{
			DLOG("  reading nodes");
			for(const auto &node : scene["nodes"]) nodes.push_back(node.get<dstd::USize>());
		}

		for(auto &buffile : bufferFiles)
		{
			buffers.push_back(Buffer {&buffile, false, nullptr});
		}

		DLOG("reading buffer views");
		for(const auto &view : gltf["bufferViews"])
		{
			auto buffer     = view["buffer"].get<dstd::USize>();
			auto byteLength = view["byteLength"].get<dstd::USize>();
			auto byteOffset = view["byteOffset"].get<dstd::USize>();

			bufferViews.push_back(BufferView {&buffers[buffer], byteOffset, byteLength});
		}

		DLOG("reading accessors");
		for(const auto &acc : gltf["accessors"])
		{
			auto bufferView    = acc["bufferView"].get<dstd::USize>();
			auto componentType = static_cast<AccessorComponentType>(acc["componentType"].get<int>());
			auto type          = accessorTypeFromString(acc["type"].get<std::string>());
			auto count         = acc["count"].get<dstd::USize>();
			auto byteOffset    = (dstd::USize)0;

			accessors.push_back(Accessor {&bufferViews[bufferView], componentType, type, count, byteOffset});
		}

		DLOG("reading materials");
		for(const auto &mat : gltf["materials"])
		{
			auto name = mat["name"].get<std::string>();

			materials.push_back(Material {name});
		}

		DLOG("reading nodes");
		for(auto nodeIndex : nodes)
		{
			DLOG("  reading node #" << nodeIndex);
			auto &node       = gltf["nodes"][nodeIndex];
			auto meshIndex   = node["mesh"].get<dstd::USize>();
			const auto &mesh = gltf["meshes"][meshIndex];
			DLOG("    reading primitives");
			for(const auto &prim : mesh["primitives"])
			{
				DLOG("      reading attributes");
				DLOG("        reading POSITION");
				auto position = prim["attributes"]["POSITION"].get<dstd::USize>();
				DLOG("        reading NORMAL");
				auto normal = prim["attributes"]["NORMAL"].get<dstd::USize>();
				DLOG("        reading TEXCOORD_0");
				auto texcoord = prim["attributes"]["TEXCOORD_0"].get<dstd::USize>();

				DLOG("      reading indices");
				auto indices = prim["indices"].get<dstd::USize>();
				DLOG("      reading material");
				auto material = prim["material"].get<dstd::USize>();

				usedAccessors.insert(position);
				usedAccessors.insert(normal);
				usedAccessors.insert(texcoord);
				usedAccessors.insert(indices);

				primitives.push_back(Primitive {
				    &accessors[position],
				    &accessors[normal],
				    &accessors[texcoord],
				    &accessors[indices],
				    &materials[material],
				});
			}
		}

		for(auto a : usedAccessors) usedBufferViews.insert(accessors[a].View);
		for(auto v : usedBufferViews) usedBuffers.insert(v->Buffer);
		for(auto buffer : usedBuffers)
		{
			DLOG("Used buffer '" << buffer->File->Path << "'");
			buffer->Data      = new byte[buffer->File->Size];
			buffer->Allocated = true;
			fillBuffer(buffer);
		}

		d.Meshes.reserve(primitives.size());
		for(const auto &prim : primitives)
		{
			DLOG("Loading primitive " << d.Meshes.size());
			graphics::ModelMeshData md {};
			md.Mesh.Stride = sizeof(float) * (3 + 3 + 2);
			md.Mesh.Indices.reserve(prim.Indices->Count);
			md.Mesh.VertexData.reserve(prim.Position->Count * getIndexSize(prim.Position) +
			                           prim.Normal->Count * getIndexSize(prim.Normal) +
			                           prim.TexCoord->Count * getIndexSize(prim.TexCoord));

			// TODO: Somehow optimize the conversion from short to int
			for(dstd::USize i = 0; i < prim.Indices->Count; ++i)
			{
				auto actual = computeActualOffset(prim.Indices, i);
				// Assume the buffer is allocated. TODO: assert the buffer's allocated == true

				auto index = convertBytesAt<dstd::UInt32>(prim.Indices, actual);

				md.Mesh.Indices.push_back(index);
			}

			const auto pushFloat = [&](float f)
			{
				byte *bytes = reinterpret_cast<byte *>(&f);
				md.Mesh.VertexData.push_back(bytes[0]);
				md.Mesh.VertexData.push_back(bytes[1]);
				md.Mesh.VertexData.push_back(bytes[2]);
				md.Mesh.VertexData.push_back(bytes[3]);
			};

			const auto pushVec2 = [&](const glm::vec2 &v2)
			{
				pushFloat(v2[0]);
				pushFloat(v2[1]);
			};

			const auto pushVec3 = [&](const glm::vec3 &v3)
			{
				pushFloat(v3[0]);
				pushFloat(v3[1]);
				pushFloat(v3[2]);
			};

			// Assume Position, Normal and TexCoord have the same count. (as they should)
			for(dstd::USize i = 0; i < prim.Position->Count; ++i)
			{
				// 0 - Position
				// 1 - Normal
				// 2 - TexCoord
				dstd::USize actual[3] = {
				    computeActualOffset(prim.Position, i),
				    computeActualOffset(prim.Normal, i),
				    computeActualOffset(prim.TexCoord, i),
				};

				glm::vec3 position = convertElementAt<float, 3>(prim.Position, actual[0]);
				glm::vec3 normal   = convertElementAt<float, 3>(prim.Normal, actual[1]);
				glm::vec2 texcoord = convertElementAt<float, 2>(prim.TexCoord, actual[2]);

				pushVec3(position);
				pushVec3(normal);
				pushVec2(texcoord);
			}

			d.Textures.push_back(graphics::TextureIdSlot {prim.Material->Name, "DCore.Texture.Unknown"});

			d.Meshes.push_back(md);
		}

		DLOG("Deallocating buffers");
		for(auto buffer : usedBuffers)
			if(buffer->Allocated) delete buffer->Data; // no need to call the destructors, can delete void*

		DLOG("Done!");
		// TODO: Errors!
		return true;
	}
} // namespace dcore::util
