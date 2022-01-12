#include <dcore/Graphics/GUI/Widgets/ScrollingText.hpp>
#include <dcore/Core/Log.hpp>
#include <dcore/Core/Preferences.hpp>
#include <dcore/Event/TimeManager.hpp>

namespace dcore::graphics::gui::common
{
	ScrollingText::ScrollingText(const std::vector<std::string> &lines, const glm::vec3 &color)
	{
		HasBackground_  = false;
		BackgroundTint_ = color;
		Lines_          = lines;

		Construct_();
	}

	ScrollingText::ScrollingText(const std::vector<std::string> &lines,
	                             const dcore::resource::Resource<dcore::graphics::RTexture> &bg, const glm::vec3 &tint)
	{
		HasBackground_  = true;
		Background_     = bg;
		BackgroundTint_ = tint;
		Lines_          = lines;

		Construct_();
	}

	void ScrollingText::Construct_()
	{
		SetPosition(glm::vec2(0, 0));
		SetSize(Preferences::Instance()->GetDisplaySettings().Resolution);
		GetQuad().Color = glm::vec4(BackgroundTint_, 1);

		if(HasBackground_) GetQuad().Texture = Background_.Get();

		DCORE_LOG_INFO << "Done Creating";
	}

	void ScrollingText::Initialize()
	{
		fprintf(stderr, "ScrollingText::Initialize\n");
		Font_          = resource::ResourceManager::Instance()->Get<Font>("DCore.Font.Debug").Get();
		CurrentOffset_ = 0;
	}

	void ScrollingText::DeInitialize() {}

	void ScrollingText::Render(GuiGraphics *g)
	{
		fprintf(stderr, "Rendering\n");
		g->RenderQuad(GetQuad());
		float fontSize  = 32.0f;
		float fontScale = fontSize / Font_->GetPixelHeight();
		float offset    = 0;

		for(const auto &line : Lines_)
		{
			float width = Font_->GetTextWidth(line.c_str(), fontScale);
			float pos   = (GetQuad().Scale.x + width) / 2.0f;

			g->RenderText(Font_, line.c_str(), glm::vec2(pos, CurrentOffset_ + offset), -1.0f, fontScale);
			offset += Font_->GetLineGap() * fontScale;
		}
	}

	void ScrollingText::HandleEvent(event::Event *ev) {}

} // namespace dcore::graphics::gui::common
