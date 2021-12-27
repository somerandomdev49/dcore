#include <dcore/Renderer/Impl/OpenGL/ShaderProgram.hpp>
#include <dcore/Core/Log.hpp>
namespace dcore::graphics::impl
{
	static const char *ConvertShaderTypeToString(opengl::ShaderType type)
	{
		switch(type)
		{
		case opengl::FragmentShader: return "Fragment";
		case opengl::VertexShader: return "Vertex";
		default: return "<Error>";
		}
	}

	static int ConvertShaderTypeToIndex(opengl::ShaderType type)
	{
		switch(type)
		{
		case opengl::FragmentShader: return 1;
		case opengl::VertexShader: return 0;
		default: return -1;
		}
	}

	void opengl::ShaderProgram::Create()
	{
		Id_ = glCreateProgram();
		for(int i = 0; i < 2; ++i) ShaderIds_[i] = (UInt)-1;
	}

	void opengl::ShaderProgram::AttachShader(ShaderType type, const std::string &source)
	{
		const char *src = source.c_str();
		UInt s          = glCreateShader(type);
		glShaderSource(s, 1, &src, nullptr);
		glCompileShader(s);

		int shaderStatus;
		char errors[512];
		glGetShaderiv(s, GL_COMPILE_STATUS, &shaderStatus);
		if(!shaderStatus)
		{
			glGetShaderInfoLog(s, 512, NULL, errors);
			DCORE_LOG_ERROR << ConvertShaderTypeToString(type) << " shader compilation error:\n"
			                << errors;
			return;
		}

		glAttachShader(Id_, s);
		ShaderIds_[ConvertShaderTypeToIndex(type)] = s;
	}

	void opengl::ShaderProgram::Link()
	{
		glLinkProgram(Id_);

		int shaderStatus;
		char errors[512];
		glGetProgramiv(Id_, GL_LINK_STATUS, &shaderStatus);
		if(!shaderStatus)
		{
			glGetProgramInfoLog(Id_, 512, NULL, errors);
			DCORE_LOG_ERROR << "Linking error:\n" << errors;
			return;
		}

		for(int i = 0; i < 2; ++i) glDeleteShader(ShaderIds_[i]);
	}

	void opengl::ShaderProgram::Delete() { glDeleteProgram(Id_); }
} // namespace dcore::graphics::impl
