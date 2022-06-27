#pragma once
#include <glm/glm.hpp>

namespace dcore::graphics
{
	class RSkyBox
	{
		friend class Renderer;
		friend class RenderResourceManager;	
		void *Data_;
		glm::vec3 Color_;
	public:
		const glm::vec3 &GetColor() const { return Color_; }
	};
} // namespace dcore::graphics
