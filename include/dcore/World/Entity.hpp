#pragma once
#include <dcore/Data/FileOutput.hpp>
#include <dcore/Core/SparseSet.hpp>
#include <dcore/Core/Log.hpp>
#include <type_traits>
#include <typeindex>

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
		std::vector<System> GetSystems(const EntityHandle &entity);

		template<typename ComponentType>
		ComponentType &GetComponent(const EntityHandle &entity);

		template<typename ComponentType>
		ComponentType &AddComponent(const EntityHandle &entity, const ComponentType &comp);

		template<typename ComponentType>
		const std::vector<EntityHandle> &GetEntities() const;

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
		size_t NextAvailable_;
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
				DCORE_LOG_INFO << "Registering component " << ThisComponentName();
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
			if constexpr(!std::is_member_function_pointer<decltype(&T::Start)>::value) return;
			T &comp = ECSInstance()->GetComponent<T>(self);
			comp.Start(self);
		}

		static void Update(const EntityHandle &self)
		{
			if constexpr(!std::is_member_function_pointer<decltype(&T::Update)>::value) return;
			T &comp = ECSInstance()->GetComponent<T>(self);
			comp.Update(self);
		}

		static void End(const EntityHandle &self)
		{
			if constexpr(!std::is_member_function_pointer<decltype(&T::End)>::value) return;
			T &comp = ECSInstance()->GetComponent<T>(self);
			comp.End(self);
		}

		static void Save(const EntityHandle &self, data::Json &output)
		{
			if constexpr(!std::is_member_function_pointer<decltype(&T::Save)>::value) return;
			T &comp = ECSInstance()->GetComponent<T>(self);
			comp.Save(self, output);
		}

		static void Load(const EntityHandle &self, const data::Json &input)
		{
			if constexpr(!std::is_member_function_pointer<decltype(&T::Load)>::value) return;
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
		return *(ComponentType *)c;
	}

	template<typename ComponentType>
	const std::vector<EntityHandle> &ECS::GetEntities() const
	{
		return this->AllEntities_;
	}
}; // namespace dcore::world
