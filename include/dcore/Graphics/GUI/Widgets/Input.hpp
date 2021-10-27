#pragma once
#include <dcore/Graphics/GUI/Widget.hpp>

namespace dcore::graphics::gui::common
{
	class Input : public Widget
	{
	public:
		virtual void Initialize() override;
		virtual void DeInitialize() override;
		virtual void Render(GuiGraphics *g) override;
		virtual void HandleEvent(event::Event *e) override;

		// TODO(->Idkarn):Implement
		const std::string &GetValue();

	private:
		std::string value;
	};
} // namespace dcore::graphics::gui::common
