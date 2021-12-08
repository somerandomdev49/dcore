#include <dcore/Graphics/GUI/Widgets/Screen.hpp>

using namespace dcore::graphics::gui::common;

Screen::Screen(const glm::vec2 &size)
{
	SetPosition(glm::vec2(0, 0));
	SetSize(size);
	GetQuad().Color = glm::vec4(0, 0, 0, 0);
}

void Screen::Initialize() {}
void Screen::DeInitialize() {}

void Screen::Render(GuiGraphics *g)
{
	// Renders the quad (due to color's alpha being 0 renders nothing!)
	g->RenderQuad(GetQuad());
}

void Screen::HandleEvent(event::Event *ev) {}
