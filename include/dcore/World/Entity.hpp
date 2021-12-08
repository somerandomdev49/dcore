#pragma once
#include <dcore/Util/TypeTraits.hpp>
#include <dcore/Data/FileOutput.hpp>
#include <dcore/Core/SparseSet.hpp>
#include <dcore/Util/Debug.hpp>
#include <dcore/Core/Log.hpp>
#include <dcore/Uni.hpp>
#include <typeindex>
#include <cstdio>

namespace dcore::world
{
	using EntityHandle = uint32_t;

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
	};

	namespace detail
	{
		DCORE_HAS_MEMBER(Update);
		DCORE_HAS_MEMBER(Start);
		DCORE_HAS_MEMBER(End);
		DCORE_HAS_MEMBER(Save);
		DCORE_HAS_MEMBER(Load);
	}

	template<class S>
	System GetSystem()
	{
		return {&S::Start, &S::Update, &S::End, &S::Save, &S::Load};
	}

	class ECS
	{
	public:
		template<typename ComponentType>
		void RegisterSystem(System &&system);

		EntityHandle CreateEntity();

		/**
		 * @brief Get every registered system
		 * 
		 * @return Vector of every registered system
		 */
		const std::vector<System> &GetAllSystems() const;

		/**
		 * @brief Get all of entities in the ECS.
		 * 
		 * @return Vector of every entity
		 */
		const std::vector<EntityHandle> &GetAllEntities() const;

		/** Note: expensive method? */
		std::vector<const System*> GetSystems(const EntityHandle &entity);

		template<typename ComponentType>
		ComponentType &GetComponent(const EntityHandle &entity);

		template<typename ComponentType>
		ComponentType &AddComponent(const EntityHandle &entity, const ComponentType &comp);

		template<typename ComponentType>
		std::vector<EntityHandle> GetEntities() const;

	private:
		using ComponentHandle = void *;

		struct ComponentPool
		{
			SparseDataSet<ComponentHandle> Set_;
			size_t SystemIndex_;
		};

		std::unordered_map<std::type_index, ComponentPool> ComponentPools_;
		std::vector<System> AllSystems_;
		std::vector<EntityHandle> AllEntities_;
		std::vector<ComponentHandle> AllComponents_;
		size_t NextAvailable_ = 0;
	};

	ECS *ECSInstance(bool set = false, ECS *newECS = nullptr);

#define DCORE_COMPONENT_NAME(T, N)                    \
	template<>                                        \
	const char *ComponentBase<T>::ThisComponentName() \
	{                                                 \
		return N;                                     \
	}
#define DCORE_COMPONENT_AUTO_NAME(T)                  \
	template<>                                        \
	const char *ComponentBase<T>::ThisComponentName() \
	{                                                 \
		return #T;                                    \
	}
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
			Reg()
			{
				printf("Registering component %s.\n", ThisComponentName());
				ECSInstance()->RegisterSystem<T>(GetSystem<ComponentBase<T>>());
			}
		} RegStatic_;

	public:
		/**
		 * @brief Returns the name of the component class for the current template instance
		 *
		 * @return The name of the component class if specialized, otherwise the typeinfo name.
		 */
		static const char *ThisComponentName() { return typeid(T).name(); }

		static void Start(const EntityHandle &self)
		{
			// Do not run if the Component type doesn't implement Start.
			if constexpr(!detail::has_Start<T>()) return;
			T &comp = ECSInstance()->GetComponent<T>(self);
			comp.Start(self);
		}

		static void Update(const EntityHandle &self)
		{
			// if constexpr(!detail::has_Update<T>()) return;
			T &comp = ECSInstance()->GetComponent<T>(self);
			comp.Update(self);
		}

		static void End(const EntityHandle &self)
		{
			if constexpr(!detail::has_End<T>()) return;
			T &comp = ECSInstance()->GetComponent<T>(self);
			comp.End(self);
		}

		static void Save(const EntityHandle &self, data::Json &output)
		{
			if constexpr(!detail::has_Save<T>()) return;
			T &comp = ECSInstance()->GetComponent<T>(self);
			comp.Save(self, output);
		}

		static void Load(const EntityHandle &self, const data::Json &input)
		{
			if constexpr(!detail::has_Load<T>()) return;
			T &comp = ECSInstance()->GetComponent<T>(self);
			comp.Load(self, input);
		}
	};

	template<typename ComponentType>
	ComponentType &ECS::GetComponent(const EntityHandle &entity)
	{
		return *(ComponentType *)this->ComponentPools_[std::type_index(typeid(ComponentType))].Set_[entity];
	}

	template<typename ComponentType>
	ComponentType &ECS::AddComponent(const EntityHandle &entity, const ComponentType &comp)
	{
		auto c = new byte[sizeof(ComponentType)];
		std::memcpy(c, &comp, sizeof(ComponentType));
		this->AllComponents_.push_back(c);
		this->ComponentPools_[std::type_index(typeid(ComponentType))].Set_.Set(entity, ComponentHandle {c});
		printf("Component pool for type %s has %ld entities.\n",
			util::Debug::Demangle(typeid(ComponentType).name()).c_str(),
			this->ComponentPools_[std::type_index(typeid(ComponentType))].Set_.GetPacked().size());
		return *(ComponentType *)c;
	}

	template<typename ComponentType>
	std::vector<EntityHandle> ECS::GetEntities() const
	{
		std::vector<EntityHandle> entities;
		const auto &packed = this->ComponentPools_.at(std::type_index(typeid(ComponentType))).Set_.GetPacked();
		for(const auto &p : packed)
			entities.push_back(p.first);
		return entities;
	}
	
	template<typename ComponentType>
	void ECS::RegisterSystem(System &&system)
	{
		ComponentPools_[std::type_index(typeid(ComponentType))].SystemIndex_ = AllSystems_.size();
		AllSystems_.push_back(std::move(system));
	}
}; // namespace dcore::world
