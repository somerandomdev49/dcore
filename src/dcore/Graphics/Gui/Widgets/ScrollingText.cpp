#include <dcore/Graphics/GUI/Widgets/ScrollingText.hpp>
#include <dcore/Core/Log.hpp>
#include <dcore/Core/Preferences.hpp>
#include <dcore/Event/TimeManager.hpp>
#include <dcore/Util/Colors.hpp>

namespace dcore::graphics::gui::common
{
	ScrollingText::ScrollingText(const std::vector<std::string> &lines, const glm::vec3 &color, float speed)
	{
		HasBackground_  = false;
		BackgroundTint_ = color;
		Lines_          = lines;
		Speed_          = speed;

		Construct_();
	}

	ScrollingText::ScrollingText(const std::vector<std::string> &lines,
	                             const dcore::resource::Resource<dcore::graphics::RTexture> &bg, const glm::vec3 &tint, float speed)
	{
		HasBackground_  = true;
		Background_     = bg;
		BackgroundTint_ = tint;
		Lines_          = lines;
		Speed_          = speed;

		Construct_();
	}

	void ScrollingText::Construct_()
	{
		SetPosition(glm::vec2(0, 0));
		SetSize(Preferences::Instance()->GetDisplaySettings().Resolution);
		GetQuad().Color = glm::vec4(BackgroundTint_, 1);

		if(HasBackground_) GetQuad().Texture = Background_.Get();
	}

	void ScrollingText::Initialize()
	{
		fprintf(stderr, "Initialize\n");
		Font_          = resource::GetResource<Font>("DCore.Font.Normal").Get();
		CurrentOffset_ = GetSize().y;
	}

	void ScrollingText::DeInitialize() {}

	void ScrollingText::Render(GuiGraphics *g)
	{
		fprintf(stderr, "Render\n");
		g->RenderQuad(GetQuad());
		float fontSize  = 40.0f;
		float fontScale = fontSize / Font_->GetPixelHeight();
		float offset    = Font_->GetLineGap() * fontScale;


		for(const auto &line : Lines_)
		{
			float width = Font_->GetTextWidth(line.c_str(), fontScale);
			float pos   = (GetQuad().Scale.x - width) / 2.0f;

			g->RenderText(Font_, line.c_str(), glm::vec2(pos, CurrentOffset_ + offset), -1.0f, fontScale, glm::vec4(DCORE_COLOR_GOLDEN, 1));
			offset += Font_->GetLineGap() * fontScale;
		}

		CurrentOffset_ -= Speed_ * event::TimeManager::Instance()->GetDeltaTime();
	}

	void ScrollingText::HandleEvent(event::Event *ev) {}

} // namespace dcore::graphics::gui::common
