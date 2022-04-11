#include <dg/Loaders/MainWorldLoader.hpp>

#if 0 // Future API
#include <dg/Networking/ServerInterface.hpp>
#endif

#include <dcore/Data/FileInput.hpp>
#include <dcore/Data/Adapters/JsonAdapter.hpp>
#include <dcore/Util/JsonConverters.hpp>

#include <vector>

namespace dg::loaders
{
	MainWorldLoader::MainWorldLoader(std::string name) : Name_(std::move(name)) {}

	void MainWorldLoader::Load(dcore::world::World *world)
	{
		// Read the world data from a json file.
		dcore::data::FileInput input("data", "dcore/main/worlds/" + Name_ + ".json");
		dcore::data::adapters::JsonInputAdapter jsonAdapter;
		input.SetAdapter(&jsonAdapter);
		input.Read();

		// Add all of the necessary entities to the world
		PopulateWorld_(input, world);
	}

	void MainWorldLoader::UnLoad(dcore::world::World *world) {}

	void MainWorldLoader::PopulateWorld_(const dcore::data::FileInput &input, dcore::world::World *world)
	{
		// Create the terrain entity (holds the TerrainComponent)
		// auto terrainEntity = world->CreateEntity();
		// terrainEntity.AddComponent(dcore::world::TerrainComponent(
		//     ));

		// world->SetTerrain(
		// 	&terrainEntity.GetComponent<dcore::world::TerrainComponent>()
		// 		->GetTerrain());

		world->CreateTerrain(dcore::resource::GetResource<dcore::terrain::Heightmap>("DCore.Heightmap." + Name_));

		// Load the rest of the entities (saved)
		LoadStaticEntities_({ input.Get(), world });

		// LOG_F(INFO, "Terrain chunk count: %lld",
		// 	terrainEntity.GetComponent<dcore::world::TerrainComponent>()->GetTerrain().GetChunks().size());

		// TODO: Have a separate method for loading static entities.

#if 0 // Future API?
		auto server = dg::net::ServerInterface::Instance();

		dstd::UInt32 entityCount = 0;
		server->GetAllEntities(&entityCount, nullptr, Name_);
		std::vector<dg::net::EntityCreateData> entityData(entityCount);
		server->GetAllEntities(nullptr, &entityData[0], Name_);

		this->ConstructEntities_(entityData);
#endif
	}

	void MainWorldLoader::LoadStaticEntities_(LoadInfo info)
	{
		(void)Name_;

		const auto &objsJson = info.Json["statics"];
		for(const auto &obj : objsJson)
		{
			LOG_F(INFO, "Creating entity");
			auto entity = info.World->CreateEntity();
			
			dstd::UUID uuid;
			dstd::UUID::Parse(uuid, obj["uuid"]);
			entity.AddComponent(dcore::world::UUIDComponent(uuid));

			if(obj["type"] == "Model") LoadStaticEntity_Model_(entity, { obj, info.World });
			if(obj["type"] == "NPC") LoadStaticEntity_NPC_(entity, { obj, info.World });
			if(obj["type"] == "Interactable") LoadStaticEntity_Interactable_(entity, { obj, info.World });
			
			LOG_F(INFO, "Created entity! %lu", entity.GetId());
		}
	}

	void MainWorldLoader::LoadStaticEntity_Transform_(dcore::world::Entity &entity, LoadInfo info)
	{
		(void)Name_;
		
		glm::vec3 position =
		{
			info.Json["position"][0].get<float>(),
			info.Json["position"][1].get<float>(),
			info.Json["position"][2].get<float>(),
		};
		
		glm::quat rotation =
		{
			info.Json["rotation"][3].get<float>(),
			info.Json["rotation"][0].get<float>(),
			info.Json["rotation"][1].get<float>(),
			info.Json["rotation"][2].get<float>(),
		};

		glm::vec3 scale =
		{
			info.Json["scale"][0].get<float>(),
			info.Json["scale"][1].get<float>(),
			info.Json["scale"][2].get<float>(),
		};

		dcore::world::TransformComponent transform;
		transform.SetPosition(position);
		transform.SetRotation(rotation);
		transform.SetScale(scale);
		transform.ReCalculateMatrix(); // faster
		
		entity.AddComponent(transform);
	}
	
	void MainWorldLoader::LoadStaticEntity_Model_(dcore::world::Entity &entity, LoadInfo info)
	{
		(void)Name_;
		LoadStaticEntity_Transform_(entity, info);
		
		auto modelId = info.Json["model"].get<const std::string>();
		
		auto model = dcore::resource::ResourceManager::Instance()
			->Get<dcore::graphics::Model>(modelId);
		
		entity.AddComponent(dcore::world::ModelComponent(model));
	}
	
	void MainWorldLoader::LoadStaticEntity_NPC_(dcore::world::Entity &entity, LoadInfo info)
	{
		(void)Name_;
	}
	
	void MainWorldLoader::LoadStaticEntity_Interactable_(dcore::world::Entity &entity, LoadInfo info)
	{
		(void)Name_;
	}
} // namespace dg::loaders
