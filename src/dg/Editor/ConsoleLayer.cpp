#include <dg/Editor/Layers.hpp>
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

} // namespace

namespace dg::editor
{
	void ConsoleLayer::OnStart() { CommandBuffer_ = AllocEmptyCString(COMMAND_BUFFER_LENGTH); }
	void ConsoleLayer::OnEnd() { delete[] CommandBuffer_; }

	void ConsoleLayer::OnRender(dcore::graphics::RendererInterface *renderer)
	{
		(void)renderer;

		ImGui::Begin("Frame Log");
		const auto &queue = dcore::FrameLog::Instance()->GetQueue();
		for(const auto &s : queue) ImGui::Text("%s", s.c_str());
		ImGui::End();

		float spacing = ImGui::GetTextLineHeightWithSpacing() + ImGui::GetStyle().FramePadding.y +
						ImGui::GetStyle().ItemSpacing.y;

		ImGui::Begin("Console");
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
}
