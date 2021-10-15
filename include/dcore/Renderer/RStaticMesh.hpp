#pragma once
#include <dcore/Renderer/Impl/OpenGL.hpp>
#include <dcore/Renderer/Impl/OpenGL/Vao.hpp>
#include <dcore/Renderer/Config.hpp>

namespace dcore::graphics
{
	class Renderer;

	namespace impl::opengl
	{
		struct StaticMesh
		{
			Vao Vao_;
		};
	}; // namespace impl::opengl

	class RStaticMesh
	{
		friend class Renderer;
		friend class RenderResourceManager;
		impl::DCORE_GRAPHICS_IMPL::StaticMesh Data_;
	};
} // namespace dcore::graphics
