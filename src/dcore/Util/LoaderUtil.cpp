#include <dcore/Util/LoaderUtil.hpp>
#include <dcore/Core/Log.hpp>
#include <dcore/Uni.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

namespace dcore::util
{

	bool LoaderUtil::LoadImage(ImageData &d, const std::string &path)
	{
		d.data = stbi_load(path.c_str(), &d.size.x, &d.size.y, &d.channels, 0);
		return d.data != nullptr;
	}

	bool LoaderUtil::LoadMesh(MeshData &d, const std::string &path, const std::string &format)
	{
		d.vertexData.clear();
		d.indices.clear();
		d.stride = 0;

		for(char c : format)
		{
			switch(c)
			{
			case 'p': d.stride += sizeof(float) * 3; break;
			case 'n': d.stride += sizeof(float) * 3; break;
			case 't': d.stride += sizeof(float) * 2; break;
			}
		}

		tinyobj::ObjReader reader;

		if(!reader.ParseFromFile(path))
		{
			if(!reader.Error().empty())
			{
				DCORE_LOG_ERROR << "[tinyobjloader] error: " << reader.Error();
			}
			std::exit(1);
		}

		if(!reader.Warning().empty())
		{
			DCORE_LOG_WARNING << "[tinyobjloader] warning: " << reader.Warning();
		}

		auto &attrib = reader.GetAttrib();
		auto &shapes = reader.GetShapes();

		const auto pushFloat = [&](float f)
		{
			byte *bytes = reinterpret_cast<byte *>(&f);
			d.vertexData.push_back(bytes[0]);
			d.vertexData.push_back(bytes[1]);
			d.vertexData.push_back(bytes[2]);
			d.vertexData.push_back(bytes[3]);
		};

		for(size_t s = 0; s < shapes.size(); s++)
		{

			size_t index_offset = 0;
			// vertices.shrink_to_fit();
			// d.vertexData.reserve(vertices.size() + shapes[s].mesh.num_face_vertices.size() * 3);
			for(size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
			{
				for(size_t v = 0; v < 3; v++)
				{
					// access to vertex
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

					for(char c : format)
					{
						switch(c)
						{
						case 'p':
						{
							tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
							tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
							tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];
							pushFloat(vx);
							pushFloat(vy);
							pushFloat(vz);
							break;
						}

						case 'n':
						{
							tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
							tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
							tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 1];
							pushFloat(nx);
							pushFloat(ny);
							pushFloat(nz);
							break;
						}

						case 't':
						{
							tinyobj::real_t tx =
							    attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
							tinyobj::real_t ty =
							    attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
							pushFloat(tx);
							pushFloat(ty);
							break;
						}
						}
					}
					d.indices.push_back(d.indices.size());
				}
				index_offset += 3;
			}
		}
		return true;
	}

	// https://stackoverflow.com/a/116220/9110517
	bool LoaderUtil::LoadFile(std::string &out, const std::string &path)
	{
		constexpr auto read_size = std::size_t {4096};
		std::ifstream stream(path);
		if(!stream) return false;
		stream.exceptions(std::ios_base::badbit);

		auto buf = std::string(read_size, '\0');
		while(stream.read(&buf[0], read_size)) out.append(buf, 0, stream.gcount());
		out.append(buf, 0, stream.gcount());

		stream.close();

		return true;
	}
} // namespace dcore::util
