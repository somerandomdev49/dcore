#pragma once
#include <dcore/Renderer/Impl/OpenGL/Vao.hpp>
#include <dcore/Uni.hpp>
#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace dcore::util
{
	struct ImageData
	{
		void      *data;
		int        channels;
		glm::ivec2 size;
	};

	struct MeshData
	{
		std::vector<uint32_t> indices;
		std::vector<byte>     vertexData;
		size_t                stride;
	};

	class LoaderUtil
	{
	public:
		/** Loads an image. */
		static bool LoadImage(ImageData &d, const std::string &path);

		/** Loads a mesh using the format for storing vertices. (p - position, n - normal, t - texcoords) */
		static bool LoadMesh(MeshData &d, const std::string &path, const std::string &format = "pnt");

		/** Loads a file into a string. */
		static bool LoadFile(std::string &out, const std::string &path);
	};
} // namespace dcore::util
