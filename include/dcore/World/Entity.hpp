#pragma once
#include <dcore/Util/TypeTraits.hpp>
#include <dcore/Data/FileOutput.hpp>
#include <dcore/Core/SparseSet.hpp>
#include <dcore/Util/Debug.hpp>
#include <dcore/Core/Type.hpp>
#include <dcore/Core/Log.hpp>
#include <dcore/Uni.hpp>
#include <typeindex>
#include <cstdio>

namespace dcore::world
{
	// If the first bit is 1 then the entity is considered dead, alive otherwise.
	using EntityHandle = dstd::UInt64;

	struct System
	{
		using StartFunctionType  = void (*)(const EntityHandle &);
		using UpdateFunctionType = void (*)(const EntityHandle &);
		using EndFunctionType    = void (*)(const EntityHandle &);
		using SaveFunctionType   = void (*)(const EntityHandle &, data::Json &);
		using LoadFunctionType   = void (*)(const EntityHandle &, const data::Json &);

		StartFunctionType StartFunction;
		UpdateFunctionType UpdateFunction;
		EndFunctionType EndFunction;
		SaveFunctionType SaveFunction;
		LoadFunctionType LoadFunction;
		std::string Name;
		const std::type_info &Type;
	};

	namespace detail
	{
		DCORE_HAS_MEMBER(Update);
		DCORE_HAS_MEMBER(Start);
		DCORE_HAS_MEMBER(End);
		DCORE_HAS_MEMBER(Save);
		DCORE_HAS_MEMBER(Load);
	} // namespace detail

	template<class S>
	System GetSystem()
	{
		return {&S::Start, &S::Update, &S::End, &S::Save, &S::Load, S::ThisComponentName(), typeid(S)};
	}

	class ECS
	{
	public:
		template<typename ComponentType>
		void RegisterSystem(System &&system);

		EntityHandle CreateEntity();

		/**
		 * @brief Create an entity with a specified uuid instead of a random one.
		 *
		 * @param uuid The UUID of the entity
		 */
		void CreateEntityWithUUId(const std::string &uuid);

		/**
		 * @brief Get every registered system
		 *
		 * @return Vector of every registered system
		 */
		const std::vector<System> &GetAllSystems() const;

		/**
		 * @brief Get all of entities in the ECS (including dead ones).
		 * @warning This includes the dead entities (msb is 1)!
		 * @deprecated Use iterators (e.g. for(... : *ECSInstance()) { ... })
		 *
		 * @return Vector of every entity (including dead ones)
		 */
		const std::vector<EntityHandle> &GetAllEntities() const;

		class AllEntityIterator
		{
		public:
			AllEntityIterator &operator++()
			{
				constexpr EntityHandle mask = (EntityHandle)1 << ((sizeof(EntityHandle) * 8) - 1);
				Index_ += 1;
				while(Bound_->AllEntities_[Index_] & mask) ++Index_;
				return *this;
			}

			bool operator==(const AllEntityIterator &other) { return other.Index_ == this->Index_; }

			bool operator!=(const AllEntityIterator &other) { return !(*this == other); }

			const EntityHandle &operator*() { return Bound_->AllEntities_[Index_]; }

			dstd::USize CurrentIndex() const { return Index_; }

		private:
			friend class ECS;
			AllEntityIterator(ECS *bound, dstd::USize index) : Index_(index), Bound_(bound) {}
			dstd::USize Index_;
			ECS *Bound_;
		};

		AllEntityIterator begin() { return AllEntityIterator(this, 0); }
		AllEntityIterator end() { return AllEntityIterator(this, AllEntities_.size()); }

		/** Note: expensive method? TODO: Convert to an iterator. */
		std::vector<const System *> GetSystems(const EntityHandle &entity);

		/**
		 * @brief Gets the index of a system with a specific name
		 *
		 * @param name The name of the system to get
		 * @return The index to a system (meaningless to user).
		 */
		dstd::USize GetSystemByName(const std::string &name) const;

		/**
		 * @brief Gets the pointer to a system at an index.
		 * @see GetSystemByName
		 *
		 * @param index The index of a system (returned by @c GetSystemByName)
		 * @return A const pointer to the system
		 */
		const System *GetSystemByIndex(dstd::USize index) const;

		template<typename ComponentType>
		ComponentType *GetComponent(const EntityHandle &entity);

		template<typename ComponentType>
		ComponentType *AddComponent(const EntityHandle &entity, const ComponentType &comp);

		template<typename ComponentType>
		std::vector<EntityHandle> GetEntities() const;

		/**
		 * @brief Similar to add component but takes the system index.
		 * @see GetSystemByName
		 *
		 * @param index The index of the system
		 * @param entity The entity handle that will be added to the system
		 */
		void AddEntityToSystem(dstd::USize index, const EntityHandle &entity);

	private:
		struct ComponentPool
		{
			DynamicSparseDataSet Set_;
			size_t SystemIndex_;
		};

		std::unordered_map<std::type_index, ComponentPool> ComponentPools_;
		std::unordered_map<EntityHandle, std::string> UUIDMap_;
		std::vector<System> AllSystems_;
		std::vector<EntityHandle> AllEntities_;
		std::vector<void *> AllComponents_;
		size_t NextAvailable_  = 0;
		size_t AvailableCount_ = 0;
	};

	ECS *ECSInstance(bool set = false, ECS *newECS = nullptr);

#define DCORE_COMPONENT_REGISTER(T)                                \
	template<>                                                     \
	const T::ComponentBase<T>::Reg T::ComponentBase<T>::RegStatic_ \
	{                                                              \
	}

	template<typename T>
	class ComponentBase
	{
	protected:
		static const struct Reg
		{
			Reg() { ECSInstance()->RegisterSystem<T>(GetSystem<ComponentBase<T>>()); }
		} RegStatic_;

	public:
		/**
		 * @brief Returns the name of the component class for the current template instance
		 *
		 * @return The name of the component class if specialized, otherwise the typeinfo name.
		 */
		static std::string ThisComponentName() { return util::Debug::Demangle(typeid(T).name()); }

		static void Start(const EntityHandle &self)
		{
			// Do not run if the Component type doesn't implement Start.
			if constexpr(!detail::has_Start<T>()) return;
			T *comp = ECSInstance()->GetComponent<T>(self);
			// No need for NULL checking here, if this is called, the component exists.
			comp->Start(self);
		}

		static void Update(const EntityHandle &self)
		{
			if constexpr(!detail::has_Update<T>()) return;
			T *comp = ECSInstance()->GetComponent<T>(self);
			comp->Update(self);
		}

		static void End(const EntityHandle &self)
		{
			if constexpr(!detail::has_End<T>()) return;
			T *comp = ECSInstance()->GetComponent<T>(self);
			comp->End(self);
		}

		static void Save(const EntityHandle &self, data::Json &output)
		{
			if constexpr(!detail::has_Save<T>()) return;
			T *comp = ECSInstance()->GetComponent<T>(self);
			comp->Save(self, output);
		}

		static void Load(const EntityHandle &self, const data::Json &input)
		{
			if constexpr(!detail::has_Load<T>()) return;
			T *comp = ECSInstance()->GetComponent<T>(self);
			comp->Load(self, input);
		}
	};

	template<typename ComponentType>
	ComponentType *ECS::GetComponent(const EntityHandle &entity)
	{
		LOG_F(INFO, "Get component %s", util::Debug::Demangle(typeid(ComponentType).name()).c_str());
		LOG_F(INFO, "... <- %lu", entity);

		if(this->ComponentPools_.find(std::type_index(typeid(ComponentType))) == this->ComponentPools_.end())
			return nullptr;

		auto &set = this->ComponentPools_[std::type_index(typeid(ComponentType))].Set_;
		if(!set.Contains(entity)) return nullptr;

		return (ComponentType *)set[entity];
	}

	template<typename ComponentType>
	ComponentType *ECS::AddComponent(const EntityHandle &entity, const ComponentType &comp)
	{
		auto c = new byte[sizeof(ComponentType)];
		std::memcpy(c, &comp, sizeof(ComponentType));
		this->AllComponents_.push_back(c);
		// LOG_F(INFO, "Adding entity to component pool for type %s",
		// util::Debug::Demangle(typeid(ComponentType).name()).c_str());
		this->ComponentPools_[std::type_index(typeid(ComponentType))].Set_.Set(entity, ComponentHandle {c});
		printf("Component pool for type %s has %zu entities.\n",
		       util::Debug::Demangle(typeid(ComponentType).name()).c_str(),
		       this->ComponentPools_[std::type_index(typeid(ComponentType))].Set_.GetPacked().size());

		return (ComponentType *)c;
	}

	template<typename ComponentType>
	std::vector<EntityHandle> ECS::GetEntities() const
	{
		std::vector<EntityHandle> entities;
		const auto &packed = this->ComponentPools_.at(std::type_index(typeid(ComponentType))).Set_.GetPacked();
		for(const auto &p : packed) entities.push_back(p.first);
		return entities;
	}

	template<typename ComponentType>
	void ECS::RegisterSystem(System &&system)
	{
		printf("ECS::RegisterSystem()\n");
		ComponentPools_[std::type_index(typeid(ComponentType))].SystemIndex_ = AllSystems_.size();
		printf("New system index: %zu\n", AllSystems_.size());
		AllSystems_.push_back(std::move(system));
	}
}; // namespace dcore::world
