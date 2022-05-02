#include <algorithm>
#include <dcore/Graphics/GUI/Widgets/Console.hpp>
#include <dcore/Event/TimeManager.hpp>
#include <dcore/Core/Preferences.hpp>
#include <dcore/Core/FrameLog.hpp>
#include <dcore/Core/Log.hpp>
#include <imgui.h>

namespace dcore::graphics::gui::common
{

	Console::Console() = default;

	void Console::Initialize()
	{
	}

	void Console::DeInitialize()
	{
	}

	void Console::Render(GuiGraphics *g)
	{
	}

	void Console::HandleEvent(event::Event *ev) {}

} // namespace dcore::graphics::gui::common
