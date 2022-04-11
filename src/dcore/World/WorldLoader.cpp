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
		auto *loader = this->Loaders_[name];
		auto *world  = new World();

		platform::Context::Instance()->SetWorld(world);
		Loaded_ = name;

		Worlds_[name] = world;

		world->Initialize();
		loader->Load(world);
	}

	void WorldLoaderRegistry::UnLoadAllWorlds()
	{
		for(const auto &[name, _] : Loaders_)
		{
			if(Worlds_.find(name) == Worlds_.end()) continue;
			UnLoadWorld(name);
		}
	}

	void WorldLoaderRegistry::UnLoadWorld(const std::string &name)
	{
		auto *loader = this->Loaders_[name];
		auto *world  = this->Worlds_[name];

		if(name == Loaded_)
		{
			if(Worlds_.empty())
			{
				platform::Context::Instance()->SetWorld(nullptr);
				Loaded_ = "<nullptr!error>";
			}
			else
			{
				platform::Context::Instance()->SetWorld(Worlds_.begin()->second);
				Loaded_ = Worlds_.begin()->first;
			}
		}

		loader->UnLoad(world);
		world->DeInitialize();

		Worlds_.erase(name);
		delete world;
	}
} // namespace dcore::world
