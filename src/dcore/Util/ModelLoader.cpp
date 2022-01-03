#include <dcore/Util/LoaderUtil.hpp>
#include <dcore/Data/Adapters/JsonAdapter.hpp>
#include <dcore/Data/FileInput.hpp>
#include <dcore/Core/Type.hpp>
#include <dcore/Core/Log.hpp>
#include <dcore/Uni.hpp>

#include <vector>
#include <string>

namespace dcore::util
{
	namespace
	{
		enum AccessorComponentType
		{
			ACCESSOR_SIGNED_BYTE = 5120,
			ACCESSOR_UNSIGNED_BYTE = 5121,
			ACCESSOR_SIGNED_SHORT = 5122,
			ACCESSOR_UNSIGNED_SHORT = 5123,
			ACCESSOR_UNSIGNED_INT = 5125,
			ACCESSOR_FLOAT = 5126,
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

		struct BufferFile
		{
			dstd::USize Size;
			std::string Path;
		};

		struct Buffer
		{
			BufferFile *File;
			bool Allocated;
			void *Data;
		};

		struct BufferView
		{
			Buffer *buffer;
			dstd::USize ByteOffset;
			dstd::USize ByteLength;
		};

		struct Accessor
		{
			BufferView *View;
			AccessorComponentType ComponentType;
			AccessorType Type;
			dstd::USize Count;
			dstd::USize ByteOffset;
		};

		struct Primitive
		{
			Accessor *Position;
			Accessor *Normal;
			Accessor *TexCoord;
			Accessor *Indices;
			dstd::Int8 Material;
		};
	} // namespace

	bool LoaderUtil::LoadModel(ModelData &d, const std::string &folder, const std::string &gltfPath)
	{
		data::FileInput fi(folder, gltfPath);
		data::adapters::JsonInputAdapter jia;
		fi.SetAdapter(&jia);
		fi.Read();
		auto &gltf = fi.Get();

		std::vector<BufferFile> bufferFiles;
		bufferFiles.reserve(gltf["buffers"].size());

		for(const auto &buffer : gltf["buffers"])
		{
			bufferFiles.push_back(
			    BufferFile {buffer["byteLength"].get<dstd::USize>(), folder + "/" + buffer["uri"].get<std::string>()});
		}

		// Overall, we ignore scenes, nodes and meshes and only include "primitives" in output.

		std::vector<
	}
} // namespace dcore::util
