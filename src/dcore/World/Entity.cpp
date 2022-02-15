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

	void ECS::AddEntityToSystem(dstd::USize index, const EntityHandle &entity)
	{
		if(index >= this->AllSystems_.size())
		{
			LOG_F(WARNING, "Tried adding entity to system with an invalid id: %lu", index);
			return;
		}

		// TODO: IMPLEMENT THIS!
		// this->ComponentPools_.at(std::type_index(this->AllSystems_[index].Type)).Set_.Set()
	}

	dstd::USize ECS::GetSystemByName(const std::string &name) const
	{
		// TODO: Instead of doing a linear search, make a map of systems with their names,
		//       essentialy caching this thing:

		dstd::USize idx = 0;
		for(const auto &system : this->AllSystems_)
		{
			if(name == system.Name)
			{
				return idx;
			}
			++idx;
		}

		return (dstd::USize)(-1);
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

	ECS::EntityIterators ECS::GetEntities(const std::type_info &type) const
	{
		static dstd::DynamicVector emptyDynamicVector(0);

		if(this->ComponentPools_.find(std::type_index(type)) == this->ComponentPools_.end())
		{
			LOG_F(ERROR, "No component pool for type %s", util::Debug::Demangle(type.name()).c_str());
			return ECS::EntityIterators {emptyDynamicVector};
		}

		const auto &packed = this->ComponentPools_.at(std::type_index(type)).Set_.GetPacked();
		return ECS::EntityIterators {packed};
	}

} // namespace dcore::world
