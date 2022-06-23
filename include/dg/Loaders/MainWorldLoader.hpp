#pragma once
#include <dcore/World/WorldLoader.hpp>
#include <dcore/Data/FileInput.hpp>

namespace dg::loaders
{
	/**
	 * @brief Loads the main game worlds (terrain, entities, ...)
	 */
	class MainWorldLoader : public dcore::world::WorldLoader
	{
	public:
		MainWorldLoader(std::string name);

		void Load(dcore::world::World *world) override;
		void UnLoad(dcore::world::World *world) override;
		dcore::world::EntityHandle GetPlayer() const { return Player_; }

	private:
		struct LoadInfo
		{
			const dcore::data::Json &Json;
			dcore::world::World *World;
		};

		void PopulateWorld_(const dcore::data::FileInput &input, dcore::world::World *world);
		void PopulateWorldV1_(LoadInfo info);
		void LoadStaticEntities_(LoadInfo info);

		void LoadStaticEntity_Transform_(dcore::world::Entity &entity, LoadInfo info);
		void LoadStaticEntity_Model_(dcore::world::Entity &entity, LoadInfo info);
		void LoadStaticEntity_NPC_(dcore::world::Entity &entity, LoadInfo info);
		void LoadStaticEntity_Interactable_(dcore::world::Entity &entity, LoadInfo info);

		std::string Name_;
		dcore::world::EntityHandle Player_;
	};
} // namespace dg::loaders
