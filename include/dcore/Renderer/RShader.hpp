#pragma once
#include <dcore/Renderer/Impl/OpenGL.hpp>
#include <dcore/Renderer/Config.hpp>

namespace dcore::graphics
{
    class Renderer;

	namespace opengl
	{
		struct Shader { UInt Id; };
		struct Uniform { UInt Location; };
	};

	class RShader
	{
		friend class RUniform;
		friend class Renderer;
		DCORE_GRAPHICS_IMPL::Shader Data_;
	};

	class RUniform
	{
		friend class Renderer;
		friend class RShader;
		DCORE_GRAPHICS_IMPL::Uniform Data_;
	};
}

