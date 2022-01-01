#pragma once
#include <dcore/Renderer/Config.hpp>

namespace dcore::graphics
{
	/** @deprecated */
	class RStaticMesh
	{
		friend class Renderer;
		friend class RenderResourceManager;
		void *Data_;
	};
} // namespace dcore::graphics
