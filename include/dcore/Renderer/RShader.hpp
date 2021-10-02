#pragma once
#include <dcore/Renderer/Impl/OpenGL.hpp>
#include <dcore/Renderer/Config.hpp>

namespace dcore::graphics
{
    class Renderer;

	namespace opengl
	{
		struct Shader
		{
			UInt Id;
		};
	};

	class RShader
	{
		friend class Renderer;
		DCORE_GRAPHICS_IMPL::Shader Data_;
	};
}

