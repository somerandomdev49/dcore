#include <dg/Loaders/MainWorldLoader.hpp>

#if 0 // Future API
#include <dg/Networking/ServerInterface.hpp>
#endif

#include <dcore/Data/FileInput.hpp>
#include <dcore/Data/Adapters/JsonAdapter.hpp>

#include <vector>

namespace dg::loaders
{
	MainWorldLoader::MainWorldLoader(const std::string &name) : Name_(name) {}

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
		auto terrainEntity = world->CreateEntity();
		terrainEntity.AddComponent(dcore::world::TerrainComponent(
		    dcore::resource::GetResource<dcore::terrain::Heightmap>("DCore.Heightmap." + Name_)));

		// Load the rest of the entities (saved)
		world->Load(input);

		// TODO: Have a separate method for loading static entities.

#if 0 // Future API
		auto server = dg::net::ServerInterface::Instance();

		dstd::UInt32 entityCount = 0;
		server->GetAllEntities(&entityCount, nullptr, Name_);
		std::vector<dg::net::EntityCreateData> entityData(entityCount);
		server->GetAllEntities(nullptr, &entityData[0], Name_);

		this->ConstructEntities_(entityData);
#endif
	}
} // namespace dg::loaders
