#include <dcore/World/WorldLoader.hpp>
#include <dcore/Platform/Platform.hpp>

namespace dcore::world
{
	void WorldLoaderRegistry::AddWorldLoader(const std::string &name, WorldLoader *worldLoader)
	{
		this->Loaders_[name] = worldLoader;
	}

	void WorldLoaderRegistry::LoadWorld(const std::string &name)
	{
		auto loader = this->Loaders_[name];
		auto world  = new World();
		platform::Context::Instance()->SetWorld(world);
		loader->Load(world);
	}
} // namespace dcore::world
