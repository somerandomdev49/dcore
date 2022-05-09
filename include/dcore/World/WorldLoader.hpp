#pragma once
#include <dcore/World/World.hpp>

namespace dcore::world
{
	/**
	 * @brief Abstract class for loading/unloading worlds.
	 */
	class WorldLoader
	{
	public:
		virtual ~WorldLoader() = default;

		/**
		 * @brief Initializes a world
		 *
		 * @param world The world that needs to be populated (world = Context::GetWorld())
		 */
		virtual void Load(World *world) = 0;

		/**
		 * @brief Deinitializes a world
		 *
		 * @param world The world that needs to be populated (world = Context::GetWorld())
		 */
		virtual void UnLoad(World *world) = 0;
	};

	/**
	 * @brief Contains WorldLoaders mapped by name.
	 */
	class WorldLoaderRegistry
	{
	public:
		/**
		 * @brief Adds a world loader to the registry
		 *
		 * @param name The name of the world.
		 * @param worldLoader The world loader to add.
		 */
		void AddWorldLoader(const std::string &name, WorldLoader *worldLoader);

		/**
		 * @brief Loads a world (unloading the previous active world) by name.
		 *
		 * Note that the ResourceLoader is reset.
		 *
		 * @param name The name of the world to load specified in the first argument to AddWorldLoader
		 */
		void LoadWorld(const std::string &name);

		void UnLoadWorld(const std::string &name);

		void UnLoadAllWorlds();

		std::unordered_map<std::string, WorldLoader *> &GetLoaders() { return Loaders_; }
		const std::unordered_map<std::string, WorldLoader *> &GetLoaders() const { return Loaders_; }

	private:
		std::string Loaded_;
		std::unordered_map<std::string, World *> Worlds_;
		std::unordered_map<std::string, WorldLoader *> Loaders_;
	};
} // namespace dcore::world
