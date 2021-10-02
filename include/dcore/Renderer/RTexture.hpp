#pragma once
#include <dcore/Renderer/Impl/OpenGL.hpp>
#include <dcore/Renderer/Config.hpp>

namespace dcore::graphics
{
    class Renderer;

	namespace opengl
	{
		struct Texture
		{
			UInt Id;
		};
	};

	class RTexture
	{
		friend class Renderer;
		DCORE_GRAPHICS_IMPL::Texture Data_;
	};
}

