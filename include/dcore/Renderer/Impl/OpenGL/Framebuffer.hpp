#pragma once
#include <dcore/Renderer/Impl/OpenGL/GL.hpp>
#include <dcore/Core/Type.hpp>
#include <dcore/Uni.hpp>

namespace dcore::graphics
{
	class Renderer;
}

namespace dcore::graphics::impl::opengl
{
	class Framebuffer
	{
		UInt FBO_, RBO_, Texture_;
		glm::vec2 Size_;
		friend class dcore::graphics::Renderer;

	public:
		void Create(glm::vec2 size);
		void Delete();
		void Bind() const;
		void UnBind() const;
		void ReSize(glm::vec2 newSize);
	};
}
