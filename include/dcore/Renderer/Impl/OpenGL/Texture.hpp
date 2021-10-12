#pragma once
#include <dcore/Renderer/Impl/OpenGL.hpp>
#include <glm/glm.hpp>
#include <cinttypes>
#include <vector>

namespace dcore::graphics
{
	class Renderer;
}

namespace dcore::graphics::impl::opengl
{
	enum TextureType
	{
		Texture1D      = GL_TEXTURE_1D,
		Texture2D      = GL_TEXTURE_2D,
		Texture3D      = GL_TEXTURE_3D,
		TextureCubemap = GL_TEXTURE_CUBE_MAP,
	};

	enum TextureParam
	{
		TextureParamWrapS     = GL_TEXTURE_WRAP_S,
		TextureParamWrapT     = GL_TEXTURE_WRAP_T,
		TextureParamMinFilter = GL_TEXTURE_MIN_FILTER,
		TextureParamMagFilter = GL_TEXTURE_MAG_FILTER,
	};

	enum TextureParamValue
	{
		TextureWrapClampToBorder = GL_CLAMP_TO_BORDER,
		TextureWrapRepeat        = GL_REPEAT,
		TextureFilterLinear      = GL_LINEAR,
		TextureFilterNearest     = GL_NEAREST,
	};

	enum TextureFormat
	{
		TextureFormatRgba = GL_RGBA,
		TextureFormatRgb  = GL_RGB,
		TextureFormatRg   = GL_RG,
		TextureFormatR    = GL_RED,
	};

	class TextureBuffer
	{
		TextureType Type_;
		UInt Id_;
		friend class dcore::graphics::Renderer;

	public:
		void Generate(TextureType type);
		void SetParam(TextureParam param, TextureParamValue value);
		void LoadData(TextureFormat internalFormat, const glm::ivec2 &size, TextureFormat dataFormat, void *data);
		void GenMipmaps();
		void Delete();
	};
} // namespace dcore::graphics::impl::opengl
