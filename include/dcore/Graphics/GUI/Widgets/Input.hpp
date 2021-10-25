#pragma once
#include <dcore/Graphics/GUI/Widget.hpp>

namespace dcore::graphics::gui::common
{
	class Input : public Widget
	{
	public:
		const std::string &getValue();

	private:
		std::string value;
	};
} // namespace dcore::graphics::gui::common
