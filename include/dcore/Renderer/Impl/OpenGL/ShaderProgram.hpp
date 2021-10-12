#pragma once
#include <dcore/Renderer/Impl/OpenGL.hpp>
#include <cinttypes>
#include <string>

namespace dcore::graphics
{
	class Renderer;
}

namespace dcore::graphics::impl::opengl
{
	enum ShaderType
	{
		VertexShader   = GL_VERTEX_SHADER,
		FragmentShader = GL_FRAGMENT_SHADER
	};

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
} // namespace dcore::graphics::impl::opengl