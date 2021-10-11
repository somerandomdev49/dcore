#pragma once
#include <string>
#include <vector>
#include <dcore/Renderer/Impl/OpenGL/Vao.hpp>
#include <glm/glm.hpp>

namespace dcore::util
{
	struct ImageData
	{
		void *data;
		int channels;
		glm::ivec2 size;
	};

	struct MeshData
	{
		std::vector<uint32_t> indices;
		std::vector<uint8_t> verticexData;
		size_t stride;
	};

	class LoaderUtil
	{
	public:
		/** Loads an image. */
		static bool LoadImage(ImageData &d, const std::string &path);

		/** Loads a mesh using the format for storing vertices. (p - position, n - normal, t - texcoords) */
		static bool LoadMesh(MeshData &d, const std::string &path, const std::string &format = "pnt");

		/** Loads a file into a string. */
		static std::string LoadFile(const std::string &path);
	};
} // namespace dcore::util
