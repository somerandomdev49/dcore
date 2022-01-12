#include <dg/Game.hpp>
#include <dg/Entity/Actor.hpp>
#include <dg/Entity/Character.hpp>
#include <dg/Entity/CharacterController.hpp>
#include <dg/Entity/CameraFollow.hpp>

#include <dcore/Platform/Platform.hpp>
#include <dcore/Graphics/GUI/GuiManager.hpp>
#include <dcore/Graphics/GUI/Widgets/ScrollingText.hpp>

namespace
{
	std::vector<std::string> gTitles_ {
	    "DragonCore Game",
	    "LEAD PROGRAMMER: Miek",
	    "LEAD DESIGNER: Yakoyagus",
	    "PROGRAMMER: IDKARN",
	    "MUSIC: MOLODENKIY",
	    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
	    "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
	    "FILLER TEXT FILLER TEXT",
	    "TEXT FILLER FILLER TEXT",
	    "TEXT FILLER TEXT FILLER",
	    "FILLER TEXT TEXT FILLER",
	};
}

namespace dg
{
	void Game::Initialize()
	{
		auto world    = dcore::platform::Context::Instance()->GetWorld();
		auto renderer = dcore::platform::Context::Instance()->GetRendererInterface();
		{
			auto e = world->CreateEntity();
			e.AddComponent(entity::CharacterControllerComponent());
			// e.AddComponent(entity::CharacterComponent());
			e.AddComponent(dcore::world::TransformComponent());
			e.AddComponent(dcore::world::StaticMeshComponent(dcore::graphics::StaticMesh(
			    dcore::resource::ResourceManager::Instance()->Get<dcore::graphics::RStaticMesh>("DCore.Mesh.Cube"),
			    dcore::resource::ResourceManager::Instance()->Get<dcore::graphics::RTexture>(
			        "DCore.Texture.Main.Stone"))));
			e.AddComponent(entity::CameraFollowComponent(renderer->GetCamera()));
			renderer->GetCamera()->SetPosition(glm::vec3(0, 0, 0));
		}
		// {
		// 	auto e = world->CreateEntity();
		// 	e.AddComponent(dcore::world::TransformComponent());
		// 	e.AddComponent(dcore::world::StaticMeshComponent(dcore::graphics::StaticMesh(
		// 		dcore::resource::ResourceManager::Instance()->Get<dcore::graphics::RStaticMesh>("DCore.Model.Main.Building.House.Outside.Inn.01"),
		// 		dcore::resource::ResourceManager::Instance()->Get<dcore::graphics::RTexture>("DCore.Texture.Main.Stone")
		// 	)));
		// 	e.GetComponent<dcore::world::TransformComponent>().SetPosition(glm::vec3(10, 0, 10));
		// }
		dcore::graphics::gui::GuiManager::Instance()
		    ->GetRoot()
		    ->CreateChild<dcore::graphics::gui::common::ScrollingText>(gTitles_, glm::vec3(0, 0, 0));

		DCORE_LOG_INFO << "Done";
	}

	void Game::DeInitialize() {}
} // namespace dg
