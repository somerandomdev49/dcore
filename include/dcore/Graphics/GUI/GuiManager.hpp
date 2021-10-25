#pragma once
#include <dcore/Launch.hpp>
#include <dcore/Graphics/GUI/Widget.hpp>
#include <vector>

namespace dcore::platform
{
	class Context;
}

namespace dcore::graphics::gui
{
	class GuiManager
	{
	public:
		GuiManager *Instance() const;

		void Initialize();
		void DeInitialize();

		Widget *GetRoot() const;

	private:
		friend class launch::Launch;
		friend class platform::Context;
		void    SetInstance(GuiManager *manager);
		Widget *RootWidget_;
	};
} // namespace dcore::graphics::gui
