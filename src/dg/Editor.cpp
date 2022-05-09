#include <dg/Editor.hpp>
#include <dcore/Graphics/Graphics.hpp>
#include <dcore/World/World.hpp>
#include <dcore/Core/Type.hpp>
#include <dcore/Core/Log.hpp>
#include <dcore/Core/FrameLog.hpp>
#include <dg/Loaders/MainWorldLoader.hpp>
#include <dcore/Platform/Platform.hpp>
#include <imgui.h>

#define COMMAND_BUFFER_LENGTH 128

namespace
{
	char *AllocEmptyCString(dstd::USize size)
	{
		char *str = new char[size];
		std::fill(str, str + size, 0);
		return str;
	}

	class FramebufferDebugLayer : public dcore::world::DebugLayer
	{
		glm::vec2 LastSize_ = {0, 0};

	public:
		void OnStart() override {}

		void OnRender(dcore::graphics::RendererInterface *renderer) override
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {0, 0});
			ImGui::Begin("Viewport");
			ImVec2 contentRegionAvail2 = ImGui::GetContentRegionAvail();
			glm::vec2 contentRegionAvail {contentRegionAvail2.x, contentRegionAvail2.y};
			if(LastSize_ != contentRegionAvail)
			{
				LastSize_ = contentRegionAvail;
				renderer->GetRenderer()->SetViewport(LastSize_);
				renderer->GetCamera()->SetAspectRatio(LastSize_.x / LastSize_.y);
			}

			ImGui::Image(*(ImTextureID *)renderer->GetRenderer()->GetFramebufferData(), contentRegionAvail2,
			             ImVec2(0, 1), ImVec2(1, 0));

			ImGui::End();
			ImGui::PopStyleVar();
		}
	};

	class BaseDebugLayer : public dcore::world::DebugLayer
	{
		bool ShowImGuiDemoWindow_ = false;

	public:
		void OnRender(dcore::graphics::RendererInterface *renderer) override
		{
			ImGui::DockSpaceOverViewport(ImGui::GetWindowViewport());

			ImGui::Begin("[DEBUG] Screen Controls");
			ImGui::Checkbox("Show Demo Window", &ShowImGuiDemoWindow_);
			ImGui::End();

			if(ShowImGuiDemoWindow_) ImGui::ShowDemoWindow();
		}
	};

	class ConsoleDebugLayer : public dcore::world::DebugLayer
	{
		char *CommandBuffer_;

	public:
		void OnStart() override { CommandBuffer_ = AllocEmptyCString(COMMAND_BUFFER_LENGTH); }

		void OnEnd() override { delete[] CommandBuffer_; }

		void OnRender(dcore::graphics::RendererInterface *renderer) override
		{
			(void)renderer;

			ImGui::Begin("[DEBUG] Frame Log");
			const auto &queue = dcore::FrameLog::Instance()->GetQueue();
			for(const auto &s : queue) ImGui::Text("%s", s.c_str());
			ImGui::End();

			float spacing = ImGui::GetTextLineHeightWithSpacing() + ImGui::GetStyle().FramePadding.y +
			                ImGui::GetStyle().ItemSpacing.y;

			ImGui::Begin("[DEBUG] Console");
			ImGui::BeginChild("##Console_ScrollRegion", ImVec2(0, -spacing), false,
			                  ImGuiWindowFlags_AlwaysHorizontalScrollbar);
			const auto &messages = dstd::ILog()->GetMessages();
			for(dstd::USize index = 0; index < messages.GetSize(); ++index) ImGui::Text("%s", messages[index].c_str());
			ImGui::SetScrollY(ImGui::GetScrollMaxY());
			ImGui::EndChild();
			ImGui::InputText("##Console_Command", CommandBuffer_, COMMAND_BUFFER_LENGTH - 1);
			ImGui::SameLine();
			if(ImGui::Button("Enter"))
			{
				bool success = true;
				std::string_view cmd {CommandBuffer_};

				if(cmd.find("help") == 0)
				{
					LOG_F(INFO, "Help");
				}
				else
				{
					LOG_F(ERROR, "Unknown Command: '%s'", CommandBuffer_);
					success = false;
				}

				if(success) std::fill(CommandBuffer_, CommandBuffer_ + COMMAND_BUFFER_LENGTH, 0);
			}

			ImGui::End();
		}
	};
} // namespace

namespace dg
{
	void Editor::Initialize()
	{
		auto *mainWorldLoader = new dg::loaders::MainWorldLoader("World1");
		Registry_.AddWorldLoader("Main1", mainWorldLoader);
		Registry_.LoadWorld("Main1");
		
		auto *rend = dcore::platform::Context::Instance()->GetRendererInterface();
		auto *world = dcore::platform::Context::Instance()->GetWorld();
		world->AddDebugLayer(new BaseDebugLayer());
		world->AddDebugLayer(new FramebufferDebugLayer());
		world->AddDebugLayer(new ConsoleDebugLayer());
#define FOG_COLOR 0.23f, 0.48f, 0.74f, 1.0f
		rend->GetRenderer()->RenderToFramebuffer(true);
		rend->GetRenderer()->SetClearColor({ FOG_COLOR });
	}

	void Editor::DeInitialize()
	{
		Registry_.UnLoadAllWorlds();
		for(auto &[name, loader] : Registry_.GetLoaders())
			delete loader;
	}
}
