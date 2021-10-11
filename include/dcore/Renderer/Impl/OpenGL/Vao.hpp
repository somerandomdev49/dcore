#pragma once
#include <dcore/Renderer/Impl/OpenGL.hpp>
#include <cinttypes>
#include <vector>

namespace dcore::graphics {
	class Renderer;
}

namespace dcore::graphics::impl::opengl {
	class Vao {
		struct Attribute {
			enum { Float, Int } type;
			int count;
			int offset;
		};

		UInt VBO, VAO, EBO;

		int LastOffset_ = 0;
		std::vector<Attribute> Attributes_;

		friend class dcore::graphics::Renderer;

	public:
		void Load(const std::vector<uint32_t> &indices, const std::vector<uint8_t> &verticexData);

		void Bind();
		void CreateFloatAttribute(int count);
		void CreateIntAttribute(int count);
		void Create();

		void UnBind();
	};
} // namespace dcore::graphics::impl::opengl
