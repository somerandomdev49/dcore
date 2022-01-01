#pragma once
#include <dcore/Renderer/Impl/OpenGL/GL.hpp>
#include <cinttypes>
#include <string>

namespace dcore::graphics
{
	class Renderer;
}

namespace dcore::graphics::impl::opengl
{
	class ShaderProgram
	{
		UInt Id_;
		UInt ShaderIds_[2];
		friend class ::dcore::graphics::Renderer;

	public:
		void Create();
		void AttachShader(ShaderType type, const std::string &source);
		void Link();
		void Delete();
	};

	class Uniform
	
} // namespace dcore::graphics::impl::opengl
