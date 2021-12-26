#include <dcore/Graphics/GUI/Widgets/Console.hpp>
#include <dcore/Core/FrameLog.hpp>

namespace dcore::graphics::gui::common
{
	Console::Console()
	{
		fputs("Console::Console()\n", stderr);
		SetPosition(glm::vec2(0, 0));
		SetSize(glm::vec2(250, 500));
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
		g->RenderQuad(GetQuad());

		// Should be called only once, so we can write the FrameLog messages safely
		// and even if it wasn't called once, the pixels would match because no AA.

		const auto &queue = FrameLog::Instance()->GetQueue();

		glm::vec2 cur(0, 40);
		for(const auto &s : queue)
		{
			g->RenderText(Font_, s.c_str(), GetPosition() + cur, 24);
			cur.y += 40.0f;
		}
	}

	void Console::HandleEvent(event::Event *ev) {}

} // namespace dcore::graphics::gui::common
