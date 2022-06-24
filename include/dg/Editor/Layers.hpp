#pragma once
#include <dcore/World/World.hpp>
#include <unordered_map>

namespace dg::editor
{

	class FramebufferLayer : public dcore::world::DebugLayer
	{
		glm::vec2 LastSize_ = {0, 0};

	public:
		void OnRender(dcore::graphics::RendererInterface *renderer) override;
	};

	class EditorLayer : public dcore::world::DebugLayer
	{
		bool ShowImGuiDemoWindow_ = false;
		std::unordered_map<std::string, bool> Controls_;
		bool EnableWireframe_ = false;
		
		dcore::world::EntityHandle Player_;

		enum class Action
		{
			None,
			File_SaveWorld,
			File_OpenWorld,
			Edit_Create_NPC,
			Edit_Create_Item,
			Edit_Create_ShadowItem,
			Edit_Create_TerrainFeature,
			Edit_Character_Teleport,
		} Action_;

	public:
		EditorLayer(dcore::world::EntityHandle player) : Player_(player) {}
		void MenuBar_();
		void TeleportModal_();
		void OnRender(dcore::graphics::RendererInterface *renderer) override;
	};

	class ConsoleLayer : public dcore::world::DebugLayer
	{
		char *CommandBuffer_;

	public:
		void OnStart() override;
		void OnEnd() override;
		void OnRender(dcore::graphics::RendererInterface *renderer) override;
	};

	class ResourceBrowserLayer : public dcore::world::DebugLayer
	{
		void OnStart() override;
		void OnEnd() override;
		void OnRender(dcore::graphics::RendererInterface *renderer) override;
	};
}
