#pragma once
#include <GL/gl3w.h>

namespace dcore::graphics::impl::opengl
{
	using UInt = GLuint;
	using Int  = GLint;

	enum GLType
	{
		TypeUnsignedShort = GL_UNSIGNED_SHORT,
		TypeUnsignedByte  = GL_UNSIGNED_BYTE,
		TypeUnsignedInt   = GL_UNSIGNED_INT,
		TypeFloat         = GL_FLOAT,
	};
} // namespace dcore::graphics::impl::opengl
