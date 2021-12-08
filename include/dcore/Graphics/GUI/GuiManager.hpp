#pragma once
#include <dcore/Graphics/GUI/GuiGraphics.hpp>
#include <dcore/Graphics/GUI/Widget.hpp>
#include <dcore/Launch.hpp>
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
		static GuiManager *Instance();

		void Initialize();
		void DeInitialize();

		/**
		 * @brief Renders the root widget.
		 *
		 * @param g The renderer.
		 */
		void Render(GuiGraphics *g);

		Widget *GetRoot() const;

	private:
		friend class launch::Launch;
		friend class platform::Context;
		static void SetInstance(GuiManager *manager);

		Widget *RootWidget_;
		Widget *Focused_;
	};
} // namespace dcore::graphics::gui
