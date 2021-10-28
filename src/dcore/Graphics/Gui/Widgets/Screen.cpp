#include <dcore/Graphics/GUI/Widgets/Screen.hpp>

using namespace dcore::graphics::gui::common;

Screen::Screen(const glm::vec2 &size)
{
    SetPosition(glm::vec2(0, 0));
    SetSize(size);
}

void Screen::Initialize() {}
void Screen::DeInitialize() {}
void Screen::Render(GuiGraphics *g) { g->RenderQuad(GetQuad()); }
void Screen::HandleEvent(event::Event *ev) {}