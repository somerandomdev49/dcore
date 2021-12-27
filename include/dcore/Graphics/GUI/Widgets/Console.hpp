#pragma once
#include <dcore/Graphics/GUI/Widget.hpp>
#include <dcore/Graphics/GUI/Font.hpp>

namespace dcore::graphics::gui::common
{
	class Console : public Widget
	{
	public:
		Console();
		virtual void Initialize() override;
		virtual void DeInitialize() override;
		virtual void Render(GuiGraphics *g) override;
		virtual void HandleEvent(event::Event *e) override;

	private:
		Font *Font_; // TODO: Have a default font somewhere.
		float PrevWidth_ = 250.0f;
	};
} // namespace dcore::graphics::gui::common
