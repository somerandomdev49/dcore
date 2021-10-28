#pragma once
#include <dcore/Graphics/GUI/Widget.hpp>

namespace dcore::graphics::gui::common
{
	class Screen : public Widget
	{
	public:
		Screen(const glm::vec2 &size);
		virtual void Initialize() override;
		virtual void DeInitialize() override;
		virtual void Render(GuiGraphics *g) override;
		virtual void HandleEvent(event::Event *e) override;
	};
} // namespace dcore::graphics::gui::common
