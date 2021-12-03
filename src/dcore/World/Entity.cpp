#include <dcore/World/Entity.hpp>

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

	std::vector<System> ECS::GetSystems(const EntityHandle &entity)
	{
		
	}

} // namespace dcore::world
