#include <dcore/Graphics/GUI/GuiManager.hpp>
#include <dcore/Graphics/GUI/Widgets/Screen.hpp>
#include <dcore/Graphics/GUI/GuiGraphics.hpp>

namespace dcore::graphics::gui
{
	void GuiManager::Initialize()
	{
		Focused_    = nullptr;
		RootWidget_ = new common::Screen(glm::vec2(800, 600));
	}

	void GuiManager::InitializeRoot_() {
		RootWidget_->DoInitialize_();
	}

	void GuiManager::DeInitialize()
	{
		RootWidget_->DoDeInitialize_();
		delete RootWidget_;
		Focused_    = nullptr;
		RootWidget_ = nullptr;
	}

	void GuiManager::Render(GuiGraphics *g) { RootWidget_->DoRender_(g); }

	Widget *GuiManager::GetRoot() const { return RootWidget_; }

	static GuiManager *guimngrInstance = nullptr;
	GuiManager *GuiManager::Instance()
	{
		if(guimngrInstance == nullptr) guimngrInstance = new GuiManager();
		return guimngrInstance;
	}

	void GuiManager::SetInstance(GuiManager *newGuiManager) { guimngrInstance = newGuiManager; }
} // namespace dcore::graphics::gui
