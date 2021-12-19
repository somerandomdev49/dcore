#include <dcore/Renderer/Impl/OpenGL/Texture.hpp>

namespace dcore::graphics::impl
{

	void opengl::TextureBuffer::Generate(TextureType type)
	{
		Type_ = type;
		glGenTextures(1, &Id_);
		glBindTexture(Type_, Id_);
	}

	void opengl::TextureBuffer::SetParam(TextureParam param, TextureParamValue value)
	{
		glTexParameteri(Type_, param, value);
	}

	void opengl::TextureBuffer::LoadData(TextureFormat internalFormat, const glm::ivec2 &size, TextureFormat dataFormat,
	                                     void *data)
	{
		glTexImage2D(Type_, 0, internalFormat, size.x, size.y, 0, dataFormat, TypeUnsignedByte, data);
	}

	void opengl::TextureBuffer::GenMipmaps() { glGenerateMipmap(Type_); }

	void opengl::TextureBuffer::Delete() { glDeleteTextures(1, &Id_); }
} // namespace dcore::graphics::impl
