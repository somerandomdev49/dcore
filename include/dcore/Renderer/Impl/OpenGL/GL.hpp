#pragma once
#include <dcore/Renderer/Impl/OpenGL.hpp>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

#define _INL // __attribute__((always_inline))

/** OpenGL abstractions. */
namespace dcore::graphics::impl::opengl
{
	enum BufferBits
	{
		BufferBitColor   = GL_COLOR_BUFFER_BIT,
		BufferBitDepth   = GL_DEPTH_BUFFER_BIT,
		BufferBitStencil = GL_STENCIL_BUFFER_BIT,
	};

	enum Capability
	{
		CapabilityDepthTest   = GL_DEPTH_TEST,
		CapabilityStencilTest = GL_STENCIL_TEST,
	};

	enum TextureUnit
	{
		TextureUnit0 = GL_TEXTURE0
	};

	enum ElementType
	{
		ElementTriangles     = GL_TRIANGLES,
		ElementTriangleStrip = GL_TRIANGLE_STRIP,
		ElementTriangleFan   = GL_TRIANGLE_FAN,
		ElementQuads         = GL_QUADS,
	};

	enum BufferType
	{
		ArrayBuffer        = GL_ARRAY_BUFFER,
		ElementArrayBuffer = GL_ELEMENT_ARRAY_BUFFER
	};

	enum TextureType
	{
		Texture1D          = GL_TEXTURE_1D,
		Texture2D          = GL_TEXTURE_2D,
		Texture3D          = GL_TEXTURE_3D,
		TextureCubemap     = GL_TEXTURE_CUBE_MAP,
		TextureCubemapPosX = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		TextureCubemapNegX = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		TextureCubemapPosY = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		TextureCubemapNegY = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		TextureCubemapPosZ = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		TextureCubemapNegZ = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
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
		TextureWrapClampToBorder   = GL_CLAMP_TO_BORDER,
		TextureWrapRepeat          = GL_REPEAT,
		TextureFilterMipmapLinear  = GL_LINEAR_MIPMAP_LINEAR,
		TextureFilterMipmapNearest = GL_NEAREST_MIPMAP_LINEAR,
		TextureFilterLinear        = GL_LINEAR,
		TextureFilterNearest       = GL_NEAREST,
	};

	enum TextureFormat
	{
		TextureFormatRgba = GL_RGBA,
		TextureFormatRgb  = GL_RGB,
		TextureFormatRg   = GL_RG,
		TextureFormatR    = GL_RED,
	};

	enum ShaderType
	{
		VertexShader   = GL_VERTEX_SHADER,
		FragmentShader = GL_FRAGMENT_SHADER
	};

	enum PolygonMode
	{
		PolygonModeLine = GL_LINE,
		PolygonModeFill = GL_FILL,
	};

	enum Faces
	{
		FrontFace         = GL_FRONT,
		BackFace          = GL_BACK,
		FrontAndBackFaces = GL_FRONT_AND_BACK,
	};

	enum BufferUsage
	{
		BufferUsageStaticDraw  = GL_STATIC_DRAW,
		BufferUsageStreamDraw  = GL_STREAM_DRAW,
		BufferUsageDynamicDraw = GL_DYNAMIC_DRAW,
	};

	enum PixelStorage
	{
		PixelStorageUnpackAlignment = GL_UNPACK_ALIGNMENT
	};

	/** These functions should be called instead of native glXXX functions. */
	namespace Gl
	{
		inline void SetClearColor(const glm::vec4 &color) _INL;
		inline void Clear(BufferBits mask) _INL;
		inline void Enable(Capability capability) _INL;
		inline void Disable(Capability capability) _INL;

		inline void SetPolygonMode(Faces face, PolygonMode mode) _INL;

		inline void SetActiveTexture(UInt unit) _INL;
		inline void BindTexture(TextureType type, UInt id) _INL;

		inline void BindVertexArray(UInt id) _INL;
		inline void DrawElements(ElementType elemType, int indexCount, GlType type) _INL;
		inline void DrawArrays(ElementType elemType, int indexCount) _INL;

		inline void SetUniform(Int location, float v) _INL;
		inline void SetUniform(Int location, int v) _INL;
		inline void SetUniform(Int location, const glm::vec2 &v) _INL;
		inline void SetUniform(Int location, const glm::vec3 &v) _INL;
		inline void SetUniform(Int location, const glm::vec4 &v) _INL;
		inline void SetUniform(Int location, const glm::mat2x2 &v) _INL;
		inline void SetUniform(Int location, const glm::mat2x3 &v) _INL;
		inline void SetUniform(Int location, const glm::mat2x4 &v) _INL;
		inline void SetUniform(Int location, const glm::mat3x2 &v) _INL;
		inline void SetUniform(Int location, const glm::mat3x3 &v) _INL;
		inline void SetUniform(Int location, const glm::mat3x4 &v) _INL;
		inline void SetUniform(Int location, const glm::mat4x2 &v) _INL;
		inline void SetUniform(Int location, const glm::mat4x3 &v) _INL;
		inline void SetUniform(Int location, const glm::mat4x4 &v) _INL;
		inline Int GetUniformLocation(UInt program, const char *name) _INL;

		inline void UseProgram(UInt program) _INL;

		inline UInt GenerateBuffer() _INL;
		inline void GenerateBuffers(int count, UInt *buffers) _INL;

		inline UInt GenerateVertexArray() _INL;
		inline void GenerateVertexArray(int count, UInt *arrays) _INL;

		inline void BindBuffer(BufferType type, UInt buffer) _INL;
		inline void SetBufferData(BufferType type, SizePtr size, const void *data, BufferUsage usage) _INL;

		inline void VertexAttributePointer(UInt index, Int size, GlType type, Size stride, SizePtr offset,
		                                   bool norm = false) _INL;
		inline void EnableVertexAttributeArray(UInt index) _INL;

		inline void DeleteBuffer(UInt buffer);
		inline void DeleteBuffers(int count, UInt *buffers) _INL;

		inline void DeleteVertexArray(UInt array);
		inline void DeleteVertexArrays(int count, UInt *arrays) _INL;

		inline UInt GenerateTexture();
		inline void GenerateTextures(int count, UInt *textures) _INL;

		inline void PixelStore(PixelStorage param, int value) _INL;
		inline int GetPixelStore(PixelStorage param) _INL;

		inline void TextureParameter(TextureType type, TextureParam param, int value) _INL;
		inline void TextureParameter(TextureType type, TextureParam param, float value) _INL;
		inline void TextureParameter(TextureType type, TextureParam param, TextureParamValue value) _INL;

		inline void TextureImage2D(TextureType type, TextureFormat intFormat, const glm::ivec1 &size,
		                           TextureFormat dataFormat, GlType dataType, const void *data, Int level = 0) _INL;

		inline void TextureImage2D(TextureType type, TextureFormat intFormat, const glm::ivec2 &size,
		                           TextureFormat dataFormat, GlType dataType, const void *data, Int level = 0) _INL;

		inline void TextureImage3D(TextureType type, TextureFormat intFormat, const glm::ivec3 &size,
		                           TextureFormat dataFormat, GlType dataType, const void *data, Int level = 0) _INL;
	} // namespace Gl

	// DEFINITION
	namespace Gl
	{
		inline void SetUniform(UInt loc, float v) { glUniform1f(loc, v); }
		inline void SetUniform(UInt loc, int v) { glUniform1i(loc, v); }
		inline void SetUniform(UInt loc, const glm::vec2 &v) { glUniform2f(loc, v.x, v.y); }
		inline void SetUniform(UInt loc, const glm::vec3 &v) { glUniform3f(loc, v.x, v.y, v.z); }
		inline void SetUniform(UInt loc, const glm::vec4 &v) { glUniform4f(loc, v.x, v.y, v.z, v.w); }
		inline void SetUniform(UInt loc, const glm::mat2x2 &v)
		{
			glUniformMatrix2fv(loc, 1, GL_FALSE, glm::value_ptr(v));
		}
		inline void SetUniform(UInt loc, const glm::mat2x3 &v)
		{
			glUniformMatrix2x3fv(loc, 1, GL_FALSE, glm::value_ptr(v));
		}
		inline void SetUniform(UInt loc, const glm::mat2x4 &v)
		{
			glUniformMatrix2x4fv(loc, 1, GL_FALSE, glm::value_ptr(v));
		}
		inline void SetUniform(UInt loc, const glm::mat3x2 &v)
		{
			glUniformMatrix3x2fv(loc, 1, GL_FALSE, glm::value_ptr(v));
		}
		inline void SetUniform(UInt loc, const glm::mat3x3 &v)
		{
			glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(v));
		}
		inline void SetUniform(UInt loc, const glm::mat3x4 &v)
		{
			glUniformMatrix3x4fv(loc, 1, GL_FALSE, glm::value_ptr(v));
		}
		inline void SetUniform(UInt loc, const glm::mat4x2 &v)
		{
			glUniformMatrix4x2fv(loc, 1, GL_FALSE, glm::value_ptr(v));
		}
		inline void SetUniform(UInt loc, const glm::mat4x3 &v)
		{
			glUniformMatrix4x3fv(loc, 1, GL_FALSE, glm::value_ptr(v));
		}
		inline void SetUniform(UInt loc, const glm::mat4x4 &v)
		{
			glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(v));
		}
		inline Int GetUniformLocation(UInt program, const char *name) { return glGetUniformLocation(program, name); }

		inline void SetActiveTexture(UInt unit) { glActiveTexture(unit); }
		inline void BindTexture(TextureType type, UInt id) { glBindTexture(type, id); }
		inline void BindVertexArray(UInt vao) { glBindVertexArray(vao); }

		inline void DrawElements(ElementType mode, int indexCount, GlType type)
		{
			glDrawElements(mode, indexCount, type, 0);
		}
		inline void DrawArrays(ElementType elemType, int indexCount) { glDrawArrays(elemType, 0, indexCount); }

		inline void UseProgram(UInt program) { glUseProgram(program); }
		inline void SetPolygonMode(Faces face, PolygonMode mode) { glPolygonMode(face, mode); }

		inline UInt GenerateBuffer()
		{
			UInt b;
			glGenBuffers(1, &b);
			return b;
		}
		inline UInt GenerateVertexArray()
		{
			UInt b;
			glGenVertexArrays(1, &b);
			return b;
		}
		inline void BindBuffer(BufferType type, UInt buffer) { glBindBuffer(type, buffer); }

		inline void SetBufferData(BufferType type, SizePtr size, const void *data, BufferUsage usage)
		{
			glBufferData(type, size, data, usage);
		}

		inline void VertexAttributePointer(UInt index, Int size, GlType type, Size stride, SizePtr offset, bool norm)
		{
			glVertexAttribPointer(index, size, type, norm, stride, (void *)offset);
		}

		inline void EnableVertexAttributeArray(UInt index) { glEnableVertexAttribArray(index); }

		inline void DeleteBuffer(UInt buffer) { glDeleteBuffers(1, &buffer); }
		inline void DeleteVertexArray(UInt buffer) { glDeleteVertexArrays(1, &buffer); }

		inline void PixelStore(PixelStorage param, int value) { glPixelStorei(param, value); }
		inline int GetPixelStore(PixelStorage param)
		{
			int v;
			glGetIntegerv(param, &v);
			return v;
		}
	} // namespace Gl

} // namespace dcore::graphics::impl::opengl
