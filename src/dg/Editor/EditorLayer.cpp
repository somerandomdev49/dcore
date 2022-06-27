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
	void EditorLayer::MenuBar_()
	{
		if(ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("Save World");
			ImGui::MenuItem("Open World");
			ImGui::EndMenu();
		}

		if(ImGui::BeginMenu("Edit"))
		{
			ImGui::MenuItem("Copy", "CTRL+C");
			ImGui::MenuItem("Paste", "CTRL+V");
			ImGui::MenuItem("Cut", "CTRL+X");
			ImGui::Separator();
			ImGui::MenuItem("Undo", "CTRL+Z");
			ImGui::MenuItem("Redo", "CTRL+SHIFT+Z");
			ImGui::Separator();

			if(ImGui::BeginMenu("Create"))
			{
				ImGui::MenuItem("NPC", "CTRL+N 1");
				ImGui::MenuItem("Item", "CTRL+N 2");
				ImGui::MenuItem("Shadow Item", "CTRL+N 3");
				ImGui::MenuItem("Terrain Feature", "CTRL+N 4");
				ImGui::EndMenu();
			}

			if(ImGui::BeginMenu("Character"))
			{
				if(ImGui::MenuItem("Teleport")) Action_ = Action::Edit_Character_Teleport;
				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}
	}

	void EditorLayer::EditorLayer::TeleportModal_()
	{
		if(!ImGui::BeginPopupModal("Character Teleport")) return;
		ImGui::Text("Teleport Character to the specified coordinates");
		ImGui::Separator();

		static glm::vec3 value;
		ImGui::InputFloat3("Position", &value.x);

		if(ImGui::Button("Teleport"))
		{
			ImGui::CloseCurrentPopup();
			dcore::world::Entity(Player_, dcore::platform::Context::Instance()->GetWorld())
			    .GetComponent<dcore::world::TransformComponent>()
			    ->SetPosition(value);
		}

		ImGui::SameLine();

		if(ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	void EditorLayer::OnRender(dcore::graphics::RendererInterface *renderer)
	{
		ImGui::DockSpaceOverViewport(ImGui::GetWindowViewport());
		if(ImGui::Begin("Controls"))
		{
			if(ImGui::Checkbox("Enable Wireframe", &EnableWireframe_))
				renderer->GetRenderer()->SetWireframeMode(EnableWireframe_);
			ImGui::Checkbox("Show Demo Window", &ShowImGuiDemoWindow_);
			ImGui::Checkbox("Show Style Editor", &Controls_["StyleEditor"]);
		}
		ImGui::End();

		if(ImGui::Begin("Stats"))
		{
			const auto &stats = dcore::platform::Context::Instance()->GetStats();
			if(ImGui::BeginTable("Stats", 2))
			{
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("FPS");
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%d", stats.FPS);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("FrameBegin");
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%f", stats.FrameBeginTime);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("FrameEnd");
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%f", stats.FrameEndTime);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Update");
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%f", stats.UpdateTime);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Render");
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%f", stats.RenderTime);

				ImGui::EndTable();
			}
		}
		ImGui::End();

		if(ShowImGuiDemoWindow_) ImGui::ShowDemoWindow();
		if(Controls_["StyleEditor"]) ImGui::ShowStyleEditor(&ImGui::GetStyle());

		if(ImGui::BeginMainMenuBar())
		{
			MenuBar_();
			ImGui::EndMainMenuBar();
		}

		switch(Action_)
		{
		case Action::Edit_Character_Teleport:
			ImGui::OpenPopup("Character Teleport");
			Action_ = Action::None;
			break;
		default: break;
		}

		TeleportModal_();
	}
} // namespace dg::editor
