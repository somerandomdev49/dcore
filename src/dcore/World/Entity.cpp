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

	const std::vector<System> &ECS::GetAllSystems() const
	{
		return AllSystems_;
	}

	const std::vector<EntityHandle> &ECS::GetAllEntities() const
	{
		return AllEntities_;
	}

	std::vector<const System*> ECS::GetSystems(const EntityHandle &entity)
	{
		std::vector<const System*> systems;
		for(const auto &pool : ComponentPools_)
			if(pool.second.Set_.Contains(entity))
				systems.push_back(&AllSystems_[pool.second.SystemIndex_]);
		return systems;
	}

	EntityHandle ECS::CreateEntity()
	{
		// All entities list is full!
		if(NextAvailable_ == AllEntities_.size())
		{
			AllEntities_.push_back(AllEntities_.size());
			return AllEntities_.size() - 1;
		}
		size_t lastAv = NextAvailable_;
		NextAvailable_ = AllEntities_[NextAvailable_];
		return lastAv;
	}

} // namespace dcore::world
