#include <dcore/World/Entity.hpp>
#include <dcore/Util/Debug.hpp>
#include <dcore/Core/Assert.hpp>
#include <typeindex>

namespace dcore::world
{
	ECSComponentPoolProvider *ComponentPoolProviderInstance = nullptr;
	ECSComponentPoolProvider *ECSComponentPoolProvider::Instance()
	{
		if(ComponentPoolProviderInstance == nullptr)
			ComponentPoolProviderInstance = new ECSComponentPoolProvider();
		return ComponentPoolProviderInstance;
	}

	auto ECS::ComponentPoolWrapper::begin() -> ECS::ComponentPoolWrapper::EntityIterator
	{
		return { *this, 0 };
	}

	auto ECS::ComponentPoolWrapper::end() -> ECS::ComponentPoolWrapper::EntityIterator
	{
		return { *this, ComponentPool_->Set_.GetPacked().GetSize() };
	}

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

		UsedEntities_.Set(oldNextAvailable);
		return oldNextAvailable;
	}

	void ECS::Initialize()
	{
		LOG_F(INFO, "ECS::Initialize");
		NextAvailable_ = 0;
		LOG_F(INFO, "ComponentPoolProvider = %p, count = %lld", (void*)ECSComponentPoolProvider::Instance(),
			ECSComponentPoolProvider::Instance()->AllComponentPools_.size());
		for(const auto &pool : ECSComponentPoolProvider::Instance()->AllComponentPools_)
		{
			LOG_F(INFO, "Moving component pool %s into ECS!", util::Debug::Demangle(pool.Type.name()).c_str());
			
			ComponentPools_[std::type_index(pool.Type)] = AllComponentPools_.size();
			AllComponentPools_.emplace_back(pool.Size);
		}
	}

	void ECS::DeInitialize() {}
} // namespace dcore::world
