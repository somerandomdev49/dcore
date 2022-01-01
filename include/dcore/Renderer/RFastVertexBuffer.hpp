#pragma once

namespace dcore::graphics
{
	class RFastVertexBuffer
	{
		friend class Renderer;
		friend class RenderResourceManager;
		void *Data_;
	};
} // namespace dcore::graphics
