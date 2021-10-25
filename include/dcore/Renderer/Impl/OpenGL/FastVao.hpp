#pragma once
#include <dcore/Renderer/Impl/OpenGL/GL.hpp>
#include <dcore/Uni.hpp>
#include <cinttypes>
#include <vector>

namespace dcore::graphics
{
	class Renderer;
}

namespace dcore::graphics::impl::opengl
{
	class FastVao
	{
		UInt   VAO_, VBO_;
		size_t IndexCount_;
		friend class dcore::graphics::Renderer;

	public:
		void Load(size_t indexCount);
		void Delete();
	};
} // namespace dcore::graphics::impl::opengl
