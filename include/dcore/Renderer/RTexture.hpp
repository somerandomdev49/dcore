#pragma once

namespace dcore::graphics
{
	class RTexture
	{
		friend class Renderer;
		friend class RenderResourceManager;
		void *Data_;
	};
} // namespace dcore::graphics
