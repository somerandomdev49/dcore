#pragma once
#include <dcore/Graphics/GUI/Widget.hpp>
#include <dcore/Graphics/GUI/Font.hpp>

namespace dcore::graphics::gui::common
{
	/** @deprecated Not needed with WorldDebugLayers now */
	class Console : public Widget
	{
	public:
		Console();
		void Initialize() override;
		void DeInitialize() override;
		void Render(GuiGraphics *g) override;
		void HandleEvent(event::Event *e) override;
	private:
	};
} // namespace dcore::graphics::gui::common
