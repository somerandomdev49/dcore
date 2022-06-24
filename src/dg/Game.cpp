#include <dg/Game.hpp>
#include <dg/Entity/Actor.hpp>
#include <dg/Entity/Character.hpp>
#include <dg/Entity/CharacterController.hpp>
#include <dg/Entity/CameraFollow.hpp>
#include <dg/Loaders/MainWorldLoader.hpp>

#include <dcore/World/WorldLoader.hpp>
#include <dcore/Platform/Platform.hpp>
#include <dcore/Graphics/GUI/GuiManager.hpp>
#include <dcore/Graphics/GUI/Widgets/ScrollingText.hpp>
#include <imgui.h>

namespace
{
	std::vector<std::string> gTitles_ {
	    "DragonCore",
	    "Lead Programmer: Mike Redko",
	    "Lead Designer: Yaromir Konishev",
	    "Programmer: Kirril Ivanov",
	    "Music: Molod Enkiy",
	};
}

namespace dg
{
	void Game::Initialize()
	{
		DCORE_LOG_INFO << "Game::Initialize()";
		// auto world    = dcore::platform::Context::Instance()->GetWorld();
		// auto renderer = dcore::platform::Context::Instance()->GetRendererInterface();
		// {
		// 	auto e = world->CreateEntity();
		// 	e.AddComponent(entity::CharacterControllerComponent());
		// 	// e.AddComponent(entity::CharacterComponent());
		// 	e.AddComponent(dcore::world::TransformComponent());
		// 	e.AddComponent(dcore::world::StaticMeshComponent(dcore::graphics::StaticMesh(
		// 	    dcore::resource::GetResource<dcore::graphics::RStaticMesh>("DCore.Mesh.Cube"),
		// 	    dcore::resource::GetResource<dcore::graphics::RTexture>(
		// 	        "DCore.Texture.Main.Stone"))));
		// 	e.AddComponent(entity::CameraFollowComponent(renderer->GetCamera()));
		// 	renderer->GetCamera()->SetPosition(glm::vec3(0, 0, 0));
		// }
		// // {
		// // 	auto e = world->CreateEntity();
		// // 	e.AddComponent(dcore::world::TransformComponent());
		// // 	e.AddComponent(dcore::world::StaticMeshComponent(dcore::graphics::StaticMesh(
		// //
		// dcore::resource::GetResource<dcore::graphics::RStaticMesh>("DCore.Model.Main.Building.House.Outside.Inn.01"),
		// // 		dcore::resource::GetResource<dcore::graphics::RTexture>("DCore.Texture.Main.Stone")
		// // 	)));
		// // 	e.GetComponent<dcore::world::TransformComponent>().SetPosition(glm::vec3(10, 0, 10));
		// // }
		// dcore::graphics::gui::GuiManager::Instance()
		//     ->GetRoot()
		//     ->CreateChild<dcore::graphics::gui::common::ScrollingText>(gTitles_, glm::vec3(0, 0, 0));

		dcore::platform::Context::Instance()->GetFrame()->SetCursor(
			dcore::platform::Frame::CursorState_Normal,
			dcore::resource::GetResource<dcore::platform::RCursor>(
				"DCore.Texture.Main.Gui.Cursor.Cursor0.Open").Get()->cursor
		);
		
		dcore::platform::Context::Instance()->GetFrame()->SetCursor(
			dcore::platform::Frame::CursorState_Point,
			dcore::resource::GetResource<dcore::platform::RCursor>(
				"DCore.Texture.Main.Gui.Cursor.Cursor0.Normal").Get()->cursor
		);

		dcore::platform::Context::Instance()->GetFrame()->SetCursor(
			dcore::platform::Frame::CursorState_Down,
			dcore::resource::GetResource<dcore::platform::RCursor>(
				"DCore.Texture.Main.Gui.Cursor.Cursor0.Down").Get()->cursor
		);

		dcore::platform::Context::Instance()->GetFrame()->SetCursor(
			dcore::platform::Frame::CursorState_Grab,
			dcore::resource::GetResource<dcore::platform::RCursor>(
				"DCore.Texture.Main.Gui.Cursor.Cursor0.Fist").Get()->cursor
		);

		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
		auto *mainWorldLoader = new dg::loaders::MainWorldLoader("World1");
		Registry_.AddWorldLoader("Main1", mainWorldLoader);
		DCORE_LOG_INFO << "LoadWorld(Main1)";
		Registry_.LoadWorld("Main1");
		DCORE_LOG_INFO << "Done";
		auto *ctx = dcore::platform::Context::Instance();
		auto *rend = ctx->GetRendererInterface();
#define FOG_COLOR 0.23f, 0.48f, 0.74f, 1.0f
		rend->GetRenderer()->SetClearColor({ FOG_COLOR });
		rend->GetRenderer()->SetViewport(ctx->GetFrame()->GetFramebufferSize());
		auto viewport = rend->GetRenderer()->GetViewport();
		rend->GetCamera()->SetAspectRatio(viewport.x / viewport.y);
	}

	void Game::DeInitialize()
	{
		Registry_.UnLoadAllWorlds();
		for(auto &[name, loader] : Registry_.GetLoaders())
			delete loader;
	}
} // namespace dg
