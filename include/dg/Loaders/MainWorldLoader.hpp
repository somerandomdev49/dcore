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

	private:
		void PopulateWorld_(const dcore::data::FileInput &input, dcore::world::World *world);
		void PopulateWorldV1_(const dcore::data::Json &json, dcore::world::World *world);

		std::string Name_;
	};
} // namespace dg::loaders
