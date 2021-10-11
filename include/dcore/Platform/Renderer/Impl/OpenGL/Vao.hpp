#pragma once
#include <dcore/Renderer/Impl/OpenGL.hpp>
#include <cinttypes>
#include <vector>

namespace dcore::graphics {
	class Renderer;
}

namespace dcore::graphics::impl::opengl {
	class Vao {
		UInt VBO, VAO, EBO;
		friend class dcore::graphics::Renderer;

	public:
		void Create(const std::vector<uint32_t> &indices, const std::vector<uint8_t> &verticexData);

		void Bind();
		// void CreateIndexBuffer   (UInt id, void *data);
		// void CreateVertexBuffer  (UInt id, void *data);
		void CreateFloatAttribute(UInt id, void *data, int count);
		void CreateIntAttribute(UInt id, void *data, int count);
		void UnBind();
	};
} // namespace dcore::graphics::impl::opengl
