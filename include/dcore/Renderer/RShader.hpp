#pragma once
#include <dcore/Renderer/Config.hpp>

namespace dcore::graphics
{
	class RShader
	{
		friend class Renderer;
		friend class RenderResourceManager;
		friend class RUniform;
		void *Data_;
	};

	class RUniform
	{
		friend class Renderer;
		friend class RenderResourceManager;
		friend class RShader;
		void *Data_;
	};
} // namespace dcore::graphics
