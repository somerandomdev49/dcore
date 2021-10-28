#include <dcore/Graphics/GUI/GuiManager.hpp>
#include <dcore/Graphics/GUI/Widgets/Screen.hpp>

using namespace dcore::graphics::gui;

void GuiManager::Initialize()
{
	Focused_    = nullptr;
	RootWidget_ = new common::Screen(glm::vec2(800, 600));
}
