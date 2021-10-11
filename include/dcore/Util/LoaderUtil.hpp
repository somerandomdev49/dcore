#pragma once
#include <string>
#include <vector>
#include <dcore/Renderer/Impl/OpenGL/Vao.hpp>
#include <glm/glm.hpp>

namespace dcore::util {
	struct ImageData {
		void *data;
		int channels;
		glm::ivec2 size;
	};
	struct MeshData {
		std::vector<uint32_t> indices;
		std::vector<uint8_t> verticexData;
	};

	class LoaderUtil {
	public:
		static ImageData LoadImage(const std::string &path);
		static MeshData LoadMesh(const std::string &path);
		static std::string LoadFile(const std::string &path);
	};
} // namespace dcore::util
