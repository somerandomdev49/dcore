#pragma once
#include <dcore/Renderer/Impl/OpenGL.hpp>
#include <dcore/Renderer/Config.hpp>

namespace dcore::graphics {
	class Renderer;

	namespace impl::opengl {
		struct Texture {
			UInt Id;
		};
	}; // namespace impl::opengl

	class RTexture {
		friend class Renderer;
		impl::DCORE_GRAPHICS_IMPL::Texture Data_;
	};
} // namespace dcore::graphics
