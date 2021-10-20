#pragma once
#include <dcore/Renderer/Impl/OpenGL.hpp>
#include <dcore/Renderer/Impl/OpenGL/FastVao.hpp>
#include <dcore/Renderer/Config.hpp>

namespace dcore::graphics
{
	class Renderer;

	// TODO: Remove opengl thing from here!
	namespace impl::opengl
	{
		struct FastVertexBuffer
		{
			FastVao Vao_;
		};
	}; // namespace impl::opengl

	class RFastVertexBuffer
	{
		friend class Renderer;
		friend class RenderResourceManager;
		impl::DCORE_GRAPHICS_IMPL::FastVertexBuffer Data_;
	};
} // namespace dcore::graphics
