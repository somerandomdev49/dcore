#pragma once
#include <dcore/Renderer/Impl/OpenGL/GL.hpp>
#include <glm/glm.hpp>
#include <cinttypes>
#include <vector>

namespace dcore::graphics
{
	class Renderer;
}

namespace dcore::graphics::impl::opengl
{
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
