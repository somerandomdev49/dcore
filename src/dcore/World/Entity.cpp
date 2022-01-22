#include <dcore/World/Entity.hpp>
#include <dcore/Util/Debug.hpp>

namespace dcore::world
{
	ECS *ECSInstance(bool set, ECS *newEcs)
	{
		static ECS *i = new ECS();
		if(set) i = newEcs;
		return i;
	}

	const std::vector<System> &ECS::GetAllSystems() const { return AllSystems_; }

	const std::vector<EntityHandle> &ECS::GetAllEntities() const { return AllEntities_; }

	std::vector<const System *> ECS::GetSystems(const EntityHandle &entity)
	{
		std::vector<const System *> systems;
		for(const auto &pool : ComponentPools_)
			if(pool.second.Set_.Contains(entity)) systems.push_back(&AllSystems_[pool.second.SystemIndex_]);
		return systems;
	}

	void ECS::CreateEntityWithId(const EntityHandle &entity)
	{
		// TODO: Stub
	}

	void ECS::AddEntityToSystem(dstd::USize index, const EntityHandle &enyity)
	{
		// TODO: Stub
	}

	dstd::USize ECS::GetSystemByName(const std::string &name) const
	{
		// TODO: Stub
		return 0;
	}

	EntityHandle ECS::CreateEntity()
	{
		constexpr EntityHandle mask = ~((EntityHandle)1 << ((sizeof(EntityHandle) * 8) - 1));

		// All entities list is full!
		if(NextAvailable_ == AllEntities_.size())
		{
			NextAvailable_ += 1;
			AllEntities_.push_back(AllEntities_.size());
			return AllEntities_.size() - 1;
		}
		else
		{
			size_t lastAv  = NextAvailable_;
			NextAvailable_ = AllEntities_[NextAvailable_] & mask;
			return lastAv;
		}
	}

} // namespace dcore::world
