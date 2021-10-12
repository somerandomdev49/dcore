#pragma once
#include <dcore/Renderer/Impl/OpenGL.hpp>
#include <dcore/Renderer/Config.hpp>
#include <dcore/Renderer/Impl/OpenGL/ShaderProgram.hpp>

namespace dcore::graphics
{
	class Renderer;

	namespace impl::opengl
	{
		struct Shader
		{
			ShaderProgram Program_;
		};

		struct Uniform
		{
			UInt Location;
		};
	}; // namespace impl::opengl

	class RShader
	{
		friend class RUniform;
		friend class Renderer;
		impl::DCORE_GRAPHICS_IMPL::Shader Data_;
	};

	class RUniform
	{
		friend class Renderer;
		friend class RShader;
		impl::DCORE_GRAPHICS_IMPL::Uniform Data_;
	};
} // namespace dcore::graphics
