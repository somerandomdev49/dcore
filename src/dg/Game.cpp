#include <dg/Game.hpp>
#include <dg/Entity/Actor.hpp>
#include <dg/Entity/Character.hpp>
#include <dg/Entity/CharacterController.hpp>
#include <dg/Entity/CameraFollow.hpp>

#include <dcore/Platform/Platform.hpp>

namespace dg
{
	void Game::Initialize()
	{
		auto world = dcore::platform::Context::Instance()->GetWorld();
		auto renderer = dcore::platform::Context::Instance()->GetRendererInterface();
		{
			auto e = world->CreateEntity();
			e.AddComponent(entity::CharacterControllerComponent());
			// e.AddComponent(entity::CharacterComponent());
			e.AddComponent(dcore::world::TransformComponent());
			e.AddComponent(dcore::world::StaticMeshComponent(dcore::graphics::StaticMesh(
				dcore::resource::ResourceManager::Instance()->Get<dcore::graphics::RStaticMesh>("DCore.Mesh.Cube"),
				dcore::resource::ResourceManager::Instance()->Get<dcore::graphics::RTexture>("DCore.Texture.Main.Stone")
			)));
			e.AddComponent(entity::CameraFollowComponent(renderer->GetCamera()));
			renderer->GetCamera()->SetPosition(glm::vec3(0, 0, 0));
		}
		// {
		// 	auto e = world->CreateEntity();
		// 	e.AddComponent(dcore::world::TransformComponent());
		// 	e.AddComponent(dcore::world::StaticMeshComponent(dcore::graphics::StaticMesh(
		// 		dcore::resource::ResourceManager::Instance()->Get<dcore::graphics::RStaticMesh>("DCore.Mesh.Main.Building.House.Outside.Inn.01"),
		// 		dcore::resource::ResourceManager::Instance()->Get<dcore::graphics::RTexture>("DCore.Texture.Main.Stone")
		// 	)));
		// 	e.GetComponent<dcore::world::TransformComponent>().SetPosition(glm::vec3(10, 0, 10));
		// }


		DCORE_LOG_INFO << "Done";
	}

	void Game::DeInitialize()
	{

	}
}
