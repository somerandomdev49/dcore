#pragma once
#include <dcore/Renderer/Impl/OpenGL.hpp>
#include <dcore/Renderer/Impl/OpenGL/Vao.hpp>
#include <dcore/Renderer/Config.hpp>

namespace dcore::graphics
{
    class Renderer;

	namespace opengl
	{
		struct StaticMesh
		{
			Vao Vao_;
			void Load(const std::vector<Index> &indices,
			          const std::vector<Vertex> &vertices);
		};
	};

	class RStaticMesh
	{
		friend class Renderer;
		DCORE_GRAPHICS_IMPL::StaticMesh Data_;
	};
}

