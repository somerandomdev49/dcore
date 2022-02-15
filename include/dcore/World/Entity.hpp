#pragma once
#include <dcore/Core/DynamicSparseDataSet.hpp>
#include <dcore/Util/TypeTraits.hpp>
#include <dcore/Data/FileOutput.hpp>
#include <dcore/Core/SparseSet.hpp>
#include <dcore/Util/Debug.hpp>
#include <dcore/Core/UUID.hpp>
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

	template<class S, class U>
	System GetSystem()
	{
		return {&S::Start, &S::Update, &S::End, &S::Save, &S::Load, S::ThisComponentName(), typeid(U)};
	}

	// class ECS
	// {
	// public:
	// 	template<typename ComponentType>
	// 	void RegisterSystem(System &&system);

	// 	EntityHandle CreateEntity();

	// 	/**
	// 	 * @brief Get every registered system
	// 	 *
	// 	 * @return Vector of every registered system
	// 	 */
	// 	const std::vector<System> &GetAllSystems() const;

	// 	/**
	// 	 * @brief Get all of entities in the ECS (including dead ones).
	// 	 * @warning This includes the dead entities (msb is 1)!
	// 	 * @deprecated Use iterators (e.g. for(... : *ECSInstance()) { ... })
	// 	 *
	// 	 * @return Vector of every entity (including dead ones)
	// 	 */
	// 	const std::vector<EntityHandle> &GetAllEntities() const;

	// 	class AllEntityIterator
	// 	{
	// 	public:
	// 		AllEntityIterator &operator++()
	// 		{
	// 			constexpr EntityHandle mask = (EntityHandle)1 << ((sizeof(EntityHandle) * 8) - 1);
	// 			Index_ += 1;
	// 			while(Bound_->AllEntities_[Index_] & mask) ++Index_;
	// 			return *this;
	// 		}

	// 		bool operator==(const AllEntityIterator &other) { return other.Index_ == this->Index_; }

	// 		bool operator!=(const AllEntityIterator &other) { return !(*this == other); }

	// 		const EntityHandle &operator*() { return Bound_->AllEntities_[Index_]; }

	// 		dstd::USize CurrentIndex() const { return Index_; }

	// 	private:
	// 		friend class ECS;
	// 		AllEntityIterator(ECS *bound, dstd::USize index) : Index_(index), Bound_(bound) {}
	// 		dstd::USize Index_;
	// 		ECS *Bound_;
	// 	};

	// 	AllEntityIterator begin() { return AllEntityIterator(this, 0); }
	// 	AllEntityIterator end() { return AllEntityIterator(this, AllEntities_.size()); }

	// 	/** Note: expensive method? TODO: Convert to an iterator. */
	// 	std::vector<const System *> GetSystems(const EntityHandle &entity);

	// 	/**
	// 	 * @brief Gets the index of a system with a specific name
	// 	 *
	// 	 * @param name The name of the system to get
	// 	 * @return The index to a system (meaningless to user).
	// 	 */
	// 	dstd::USize GetSystemByName(const std::string &name) const;

	// 	/**
	// 	 * @brief Gets the pointer to a system at an index.
	// 	 * @see GetSystemByName
	// 	 *
	// 	 * @param index The index of a system (returned by @c GetSystemByName)
	// 	 * @return A const pointer to the system
	// 	 */
	// 	const System *GetSystemByIndex(dstd::USize index) const;

	// 	template<typename ComponentType>
	// 	ComponentType *GetComponent(const EntityHandle &entity);

	// 	template<typename ComponentType>
	// 	ComponentType *AddComponent(const EntityHandle &entity, const ComponentType &comp);

	// 	/**
	// 	 * @brief Holds the begin/end methods for the GetEntities() function.
	// 	 */
	// 	class EntityIterators
	// 	{
	// 		const dstd::DynamicVector &Vector_;
	// 		EntityIterators(const dstd::DynamicVector &vector) : Vector_(vector) {}

	// 		friend class ECS;
	// 	public:
	// 		class Iter
	// 		{
	// 			EntityIterators &Bound_;
	// 			dstd::USize Index_;
	// 			Iter(EntityIterators &bound, dstd::USize index) : Bound_(bound), Index_(index) {}

	// 			friend class EntityIterators;
	// 		public:
	// 			Iter &operator++() { return Index_++, *this; }
	// 			bool operator==(const Iter &other) { return other.Index_ == this->Index_; }
	// 			bool operator!=(const Iter &other) { return !(*this == other); }
	// 			const EntityHandle *operator*()
	// 			{
	// 				// Because the Vector_ contains structures like the following:
	// 				// struct { dstd::USize Index; ComponentType component; }
	// 				// we can simply get a pointer to the first element: Index_.
	// 				// TODO: investigate: should dstd::USize here be EntityHandle?
	// 				return Bound_.Vector_.Get<dstd::USize>(Index_);
	// 			}
	// 			dstd::USize CurrentIndex() const { return Index_; }
	// 		};

	// 		Iter begin() { return Iter(*this, 0); }
	// 		Iter end() { return Iter(*this, Vector_.GetSize()); }
	// 	};

	// 	template<typename ComponentType>
	// 	EntityIterators GetEntities() const;

	// 	EntityIterators GetEntities(const std::type_info &info) const;

	// 	/**
	// 	 * @brief Similar to add component but takes the system index.
	// 	 * @see GetSystemByName
	// 	 *
	// 	 * @param index The index of the system
	// 	 * @param entity The entity handle that will be added to the system
	// 	 */
	// 	void AddEntityToSystem(dstd::USize index, const EntityHandle &entity);

	// private:
	// 	struct ComponentPool
	// 	{
	// 		dstd::DynamicSparseDataSet Set_;
	// 		size_t SystemIndex_;
	// 	};

	// 	void Initialize();
	// 	void DeInitialize();
	// 	friend class World;

	// 	std::unordered_map<std::type_index, ComponentPool> ComponentPools_;
	// 	std::unordered_map<EntityHandle, std::string> UUIDMap_;
	// 	std::vector<System> AllSystems_;
	// 	std::vector<EntityHandle> AllEntities_;
	// 	std::vector<void *> AllComponents_;
	// 	size_t NextAvailable_  = 0;
	// 	size_t AvailableCount_ = 0;
	// };

	class ECS
	{
		/**
		 * @brief Returns the component of the specified type of an entity.
		 * 
		 * @tparam T The type of the component.
		 * @param entity The entity the the component is attached to.
		 * @return The pointer to the component.
		 */
		template<typename T>
		T *GetComponent(EntityHandle entity)
		{
			auto typeIndex = std::type_index(typeid(ComponentType));
			auto &found = ComponentPools_.find(typeIndex);

			if(found == ComponentPools_.end())
			{
				LOG_F(ERROR, "No component pool for type %s",
				      util::Debug::Demangle(typeid(ComponentType).name()).c_str());
				return nullptr;
			}

			return (T*)found->second.GetComponent(entity);
		}

		/**
		 * @brief Adds a component to an entity. The component is constructed in-place.
		 * 
		 * @tparam T The type of the component
		 * @tparam Args Arguments that are passed to the constructor of the component.
		 * @param entity The entity that the component will be attached to.
		 * @param args Arguments that are passed to the constructor of the component.
		 * @return The pointer to the attached component.
		 */
		template<typename T, typename ...Args>
		T *AddComponent(EntityHandle entity, Args &&...args)
		{
			auto typeIndex = std::type_index(typeid(ComponentType));
			auto &found = ComponentPools_.find(typeIndex);

			if(found == ComponentPools_.end())
			{
				LOG_F(ERROR, "No component pool for type %s",
				      util::Debug::Demangle(typeid(ComponentType).name()).c_str());
				return nullptr;
			}

			return nullptr; // TODO!
			// return (T*)found->second.AddComponent(entity);
		}

	private:
		struct ComponentPool
		{
			dstd::DynamicSparseSet Set_;

			void *GetComponent(EntityHandle entity)
			{
				if(!Set_.Contains(entity)) return nullptr;
				return Set_.RawGet((dstd::USize)entity);
			}
		};

		std::unordered_map<std::type_index, ComponentPool> ComponentPools_;
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
			Reg() { ECSInstance()->RegisterSystem<T>(GetSystem<ComponentBase<T>, T>()); }
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

	// template<typename ComponentType>
	// ComponentType *ECS::GetComponent(const EntityHandle &entity)
	// {
	// 	LOG_F(INFO, "Get component %s", util::Debug::Demangle(typeid(ComponentType).name()).c_str());
	// 	LOG_F(INFO, "... <- %lu", entity);

	// }

	// template<typename ComponentType>
	// ComponentType *ECS::AddComponent(const EntityHandle &entity, const ComponentType &comp)
	// {
	// 	auto c = new byte[sizeof(ComponentType)];
	// 	dstd::CopyBuffer(sizeof(ComponentType), c, (dstd::Byte *)&comp);
	// 	this->AllComponents_.push_back(c);
	// 	// LOG_F(INFO, "Adding entity to component pool for type %s",
	// 	// util::Debug::Demangle(typeid(ComponentType).name()).c_str());

	// 	if(this->ComponentPools_.find(std::type_index(typeid(ComponentType))) == this->ComponentPools_.end())
	// 	{
	// 		LOG_F(ERROR, "No component pool for type %s", util::Debug::Demangle(typeid(ComponentType).name()).c_str());
	// 		return nullptr;
	// 	}

	// 	auto &set = this->ComponentPools_.at(std::type_index(typeid(ComponentType))).Set_;

	// 	set.Set<ComponentType>(entity, (ComponentType *)c);

	// 	printf("Component pool for type %s has %lu entities.\n",
	// 	       util::Debug::Demangle(typeid(ComponentType).name()).c_str(), set.GetPacked().GetSize());

	// 	return (ComponentType *)c;
	// }

	// template<typename ComponentType>
	// ECS::EntityIterators ECS::GetEntities() const
	// {
	// 	static dstd::DynamicVector emptyDynamicVector(0);

	// 	if(this->ComponentPools_.find(std::type_index(typeid(ComponentType))) == this->ComponentPools_.end())
	// 	{
	// 		LOG_F(ERROR, "No component pool for type %s", util::Debug::Demangle(typeid(ComponentType).name()).c_str());
	// 		return ECS::EntityIterators {emptyDynamicVector};
	// 	}

	// 	const auto &packed = this->ComponentPools_.at(std::type_index(typeid(ComponentType))).Set_.GetPacked();
	// 	return ECS::EntityIterators {packed};
	// }

	// template<typename ComponentType>
	// void ECS::RegisterSystem(System &&system)
	// {
	// 	printf("ECS::RegisterSystem<%s>()\n", util::Debug::Demangle(typeid(ComponentType).name()).c_str());
	// 	ComponentPools_.insert(decltype(ComponentPools_)::value_type(
	// 	    std::type_index(typeid(ComponentType)),
	// 	    ComponentPool {dstd::DynamicSparseDataSet(sizeof(ComponentType)), ComponentPools_.size()}));
	// 	ComponentPools_.at(std::type_index(typeid(ComponentType))).SystemIndex_ = AllSystems_.size();
	// 	printf("New system index: %zu\n", AllSystems_.size());
	// 	AllSystems_.push_back(std::move(system));
	// }

	struct UUIDComponent : ComponentBase<UUIDComponent>
	{
		dstd::UUID Value;

		UUIDComponent(dstd::UUID &&uuid) : Value(std::move(uuid)) {}
	};
}; // namespace dcore::world
