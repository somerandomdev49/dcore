#pragma once
#include <dcore/World/WorldLoader.hpp>

namespace dg::loaders
{
	/**
	 * @brief Loads the menu worlds
	 */
	class MenuWorldLoader : public dcore::world::WorldLoader
	{
	public:
		MenuWorldLoader(const std::string &name);

		void Load(dcore::world::World *world) override;
		void UnLoad(dcore::world::World *world) override;

	private:
		std::string Name_;
	};
} // namespace dg::loaders
