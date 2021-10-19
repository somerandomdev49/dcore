#pragma once
#include <glm/glm.hpp>
#include <vector>

namespace dcore::graphics::gui
{
	class Widget
	{
	public:
		
	private:
		glm::vec2 Position_;
		glm::vec2 Size_;
		Widget *Parent;
		std::vector<Widget*> Children_;
	};
}
