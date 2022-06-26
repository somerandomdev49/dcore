#include "dcore/Core/Assert.hpp"
#include "dcore/Renderer/Impl/OpenGL/GL.hpp"
#include <dcore/Renderer/Impl/OpenGL/Texture.hpp>

namespace dcore::graphics::impl
{

	void opengl::TextureBuffer::Generate(TextureType type)
	{
		DCORE_ASSERT(
			type != TextureCubemapPosX &&
			type != TextureCubemapPosY &&
			type != TextureCubemapPosZ &&
			type != TextureCubemapNegX &&
			type != TextureCubemapNegY &&
			type != TextureCubemapNegZ, "Incorrect type for a texture.");
		Type_ = type;
		Id_ = Gl::GenerateTexture();
		Gl::BindTexture(Type_, Id_);
	}

	void opengl::TextureBuffer::SetParam(TextureParam param, TextureParamValue value)
	{
		glTexParameteri(Type_, param, value);
	}

	void opengl::TextureBuffer::LoadData(TextureFormat internalFormat, const glm::ivec2 &size, TextureFormat dataFormat,
	                                     void *data)
	{
		DCORE_ASSERT(data != nullptr, "data == nullptr");
		glTexImage2D(Type_, 0, internalFormat, size.x, size.y, 0, dataFormat, TypeUnsignedByte, data);
	}

	void opengl::TextureBuffer::LoadData(TextureType type, TextureFormat internalFormat, const glm::ivec2 &size, TextureFormat dataFormat,
	                                     void *data)
	{
		DCORE_ASSERT(Type_ == TextureCubemap, "Cannot use a different type when not using a cubemap");
		DCORE_ASSERT(
			type == TextureCubemapPosX ||
			type == TextureCubemapPosY ||
			type == TextureCubemapPosZ ||
			type == TextureCubemapNegX ||
			type == TextureCubemapNegY ||
			type == TextureCubemapNegZ, "Incorrect type for a cubemap.");
		glTexImage2D(type, 0, internalFormat, size.x, size.y, 0, dataFormat, TypeUnsignedByte, data);
	}

	void opengl::TextureBuffer::GenMipmaps() { glGenerateMipmap(Type_); }

	void opengl::TextureBuffer::Delete() { glDeleteTextures(1, &Id_); }
} // namespace dcore::graphics::impl
