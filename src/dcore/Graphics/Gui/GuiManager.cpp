#include <dcore/Graphics/GUI/GuiManager.hpp>
#include <dcore/Graphics/GUI/Widgets/Screen.hpp>
#include <dcore/Graphics/GUI/GuiGraphics.hpp>

using namespace dcore::graphics::gui;

void GuiManager::Initialize()
{
	Focused_    = nullptr;
	RootWidget_ = new common::Screen(glm::vec2(800, 600));
}

void GuiManager::DeInitialize()
{
	delete RootWidget_;
	Focused_    = nullptr;
	RootWidget_ = nullptr;
}

void GuiManager::Render(GuiGraphics *g) { RootWidget_->DoRender_(g); }

static GuiManager *guimngrInstance = nullptr;
GuiManager *GuiManager::Instance()
{
	if(guimngrInstance == nullptr) guimngrInstance = new GuiManager();
	return guimngrInstance;
}

void GuiManager::SetInstance(GuiManager *newGuiManager) { guimngrInstance = newGuiManager; }
