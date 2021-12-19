#include <dcore/Graphics/GUI/Widgets/Screen.hpp>

namespace dcore::graphics::gui::common
{
	Screen::Screen(const glm::vec2 &size)
	{
		SetPosition(glm::vec2(0, 0));
		SetSize(size);
		GetQuad().Color = glm::vec4(0, 0, 0, 0);

		// In a widget that requires specific textures:

		// this->TextureA_ = resource::ResourceManager::Get<RTexture>("gui.mywidget.texture.a");
		// this->TextureB_ = resource::ResourceManager::Get<RTexture>("gui.mywidget.texture.b");
		// GetQuad().Texture = this->TextureA_;
	}

	void Screen::Initialize() {}
	void Screen::DeInitialize() {}

	void Screen::Render(GuiGraphics *g)
	{
		// In the Button widget it may look like this:

		// if(this->IsClicked_) GetQuad().Texture = this->ClickedTexture_;
		// else GetQuad().Texture = this->NormalTexture_;

		// Renders the quad (here, due to color's alpha being 0, renders nothing!)
		g->RenderQuad(GetQuad());
	}

	void Screen::HandleEvent(event::Event *ev) {}

} // namespace dcore::graphics::gui::common
