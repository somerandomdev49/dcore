#include <dg/Editor/Layers.hpp>
#include <dcore/Graphics/Graphics.hpp>
#include <dcore/World/World.hpp>
#include <dcore/Core/Type.hpp>
#include <dcore/Core/Log.hpp>
#include <dcore/Core/FrameLog.hpp>
#include <dg/Loaders/MainWorldLoader.hpp>
#include <dcore/Platform/Platform.hpp>
#include <imgui.h>

namespace dg::editor
{
	void FramebufferLayer::OnRender(dcore::graphics::RendererInterface *renderer)
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
}
