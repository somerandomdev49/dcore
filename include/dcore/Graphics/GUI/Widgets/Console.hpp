#pragma once
#include <dcore/Graphics/GUI/Widget.hpp>

namespace dcore::graphics::gui::common
{
	class Console : public Widget
	{
		virtual void Initialize() override;
		virtual void DeInitialize() override;
		virtual void Render(GuiGraphics *g) override;
		virtual void HandleEvent(event::Event *e) override;
	};
} // namespace dcore::graphics::gui::common
