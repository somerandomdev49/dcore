#include <dcore/Graphics/GUI/Widgets/Console.hpp>
#include <dcore/Event/TimeManager.hpp>
#include <dcore/Core/Preferences.hpp>
#include <dcore/Core/FrameLog.hpp>

namespace dcore::graphics::gui::common
{
	Console::Console()
	{
		fputs("Console::Console()\n", stderr);
		SetPosition(glm::vec2(0, 0));
		SetSize(glm::vec2(250, Preferences::Instance()->GetDisplaySettings().Resolution.y));
		GetQuad().Color = glm::vec4(0, 0, 0, 0.6);
	}

	void Console::Initialize()
	{
		fputs("Console::Initialize()\n", stderr);
		Font_ = resource::ResourceManager::Instance()->Get<Font>("DCore.Font.Debug").Get();
	}

	void Console::DeInitialize() {}

	void Console::Render(GuiGraphics *g)
	{
		SetSize(glm::vec2(PrevWidth_, GetSize().y));
		g->RenderQuad(GetQuad());

		// Should be called only once, so we can write the FrameLog messages safely
		// and even if it wasn't called once, the pixels would match because no AA.

		const auto &queue = FrameLog::Instance()->GetQueue();

		// static float time = 0;
		// time += event::TimeManager::Instance()->GetDeltaTime();

		static float fontSize = 14;
		static float padding  = 10;
		float scale           = fontSize / Font_->GetPixelHeight();
		auto lineGap          = Font_->GetLineGap() * scale;

		glm::vec2 cur(padding, lineGap);

		// PrevWidth_ is the width that says how big the next rendered quad should be
		// from the quad's "prespective" this is the previous width of the text.
		// Basically the width of the quad lags one frame behind.
		// I'm too lazy to precompute the width.

		PrevWidth_ = 0;
		for(const auto &s : queue)
		{
			g->RenderText(Font_, s.c_str(), GetPosition() + cur, -1, scale, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

			auto w = Font_->GetTextWidth(s.c_str(), scale);
			if(PrevWidth_ < w) PrevWidth_ = w + padding * 2;

			cur.y += lineGap;
		}
	}

	void Console::HandleEvent(event::Event *ev) {}

} // namespace dcore::graphics::gui::common
