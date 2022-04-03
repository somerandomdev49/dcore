#include <dcore/World/Entity.hpp>
#include <dcore/Util/Debug.hpp>
#include <dcore/Core/Assert.hpp>

namespace dcore::world
{
	ECSComponentPoolProvider ComponentPoolProviderInstance;
	ECSComponentPoolProvider *ECSComponentPoolProvider::Instance() { return &ComponentPoolProviderInstance; }

	// const std::vector<System> &ECS::GetAllSystems() const { return AllSystems_; }

	// const std::vector<EntityHandle> &ECS::GetAllEntities() const { return AllEntities_; }

	// std::vector<const System *> ECS::GetSystems(const EntityHandle &entity)
	// {
	// 	std::vector<const System *> systems;
	// 	for(const auto &pool : ComponentPools_)
	// 		if(pool.second.Set_.Contains(entity)) systems.push_back(&AllSystems_[pool.second.SystemIndex_]);
	// 	return systems;
	// }

	// void ECS::AddEntityToSystem(dstd::USize index, const EntityHandle &entity)
	// {
	// 	if(index >= this->AllSystems_.size())
	// 	{
	// 		LOG_F(WARNING, "Tried adding entity to system with an invalid id: %lu", index);
	// 		return;
	// 	}

	// 	// TODO: IMPLEMENT THIS!
	// 	// this->ComponentPools_.at(std::type_index(this->AllSystems_[index].Type)).Set_.Set()
	// }

	// dstd::USize ECS::GetSystemByName(const std::string &name) const
	// {
	// 	// TODO: Instead of doing a linear search, make a map of systems with their names,
	// 	//       essentialy caching this thing:

	// 	dstd::USize idx = 0;
	// 	for(const auto &system : this->AllSystems_)
	// 	{
	// 		if(name == system.Name)
	// 		{
	// 			return idx;
	// 		}
	// 		++idx;
	// 	}

	// 	return (dstd::USize)(-1);
	// }

	// EntityHandle ECS::CreateEntity()
	// {
	// 	constexpr EntityHandle mask = ~((EntityHandle)1 << ((sizeof(EntityHandle) * 8) - 1));

	// 	// All entities list is full!
	// 	if(NextAvailable_ == AllEntities_.size())
	// 	{
	// 		NextAvailable_ += 1;
	// 		AllEntities_.push_back(AllEntities_.size());
	// 		return AllEntities_.size() - 1;
	// 	}
	// 	else
	// 	{
	// 		size_t lastAv  = NextAvailable_;
	// 		NextAvailable_ = AllEntities_[NextAvailable_] & mask;
	// 		return lastAv;
	// 	}
	// }

	// ECS::EntityIterators ECS::GetEntities(const std::type_info &type) const
	// {
	// 	static dstd::DynamicVector emptyDynamicVector(0);

	// 	if(this->ComponentPools_.find(std::type_index(type)) == this->ComponentPools_.end())
	// 	{
	// 		LOG_F(ERROR, "No component pool for type %s", util::Debug::Demangle(type.name()).c_str());
	// 		return ECS::EntityIterators {emptyDynamicVector};
	// 	}

	// 	const auto &packed = this->ComponentPools_.at(std::type_index(type)).Set_.GetPacked();
	// 	return ECS::EntityIterators {packed};
	// }

	ECS::ComponentPool::ComponentPool(dstd::USize componentSize) : Set_(componentSize) {}

	void *ECS::ComponentPool::AddComponent(EntityHandle entity, void *component)
	{
		// formatting
		return Set_.RawSet(entity, component);
	}

	void *ECS::ComponentPool::GetComponent(EntityHandle entity)
	{
		if(!Set_.Contains(entity)) return nullptr;
		return Set_.RawGet((dstd::USize)entity);
	}

	EntityHandle ECS::ComponentPool::GetEntity(dstd::USize index) { return *Set_.Get<EntityHandle>(index); }

	EntityHandle ECS::Create()
	{
		if(NextAvailable_ == AllEntities_.size())
		{
			AllEntities_.push_back(NextAvailable_++);
			return AllEntities_.back();
		}

		DCORE_ASSERT(NextAvailable_ < AllEntities_.size(), "Invalid dcore::world::ECS::NextAvailable_");

		EntityHandle oldNextAvailable = NextAvailable_;
		NextAvailable_                = AllEntities_[NextAvailable_];
		return oldNextAvailable;
	}

	void ECS::Initialize()
	{
		NextAvailable_ = 0;
		for(const auto &pool : ECSComponentPoolProvider::Instance()->AllComponentPools_)
		{
			AllComponentPools_.push_back(ComponentPool(pool.Size));
		}
	}

	void ECS::DeInitialize() {}
} // namespace dcore::world
