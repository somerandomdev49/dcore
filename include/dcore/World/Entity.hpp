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
#include <typeinfo>
#include <cstdio>

namespace dcore::world
{
	// If the first bit is 1 then the entity is considered dead, alive otherwise.
	using EntityHandle = dstd::UInt64;

	// struct System
	// {
	// 	using StartFunctionType  = void (*)(const EntityHandle &);
	// 	using UpdateFunctionType = void (*)(const EntityHandle &);
	// 	using EndFunctionType    = void (*)(const EntityHandle &);
	// 	using SaveFunctionType   = void (*)(const EntityHandle &, data::Json &);
	// 	using LoadFunctionType   = void (*)(const EntityHandle &, const data::Json &);

	// 	StartFunctionType StartFunction;
	// 	UpdateFunctionType UpdateFunction;
	// 	EndFunctionType EndFunction;
	// 	SaveFunctionType SaveFunction;
	// 	LoadFunctionType LoadFunction;
	// 	std::string Name;
	// 	const std::type_info &Type;
	// };

	// namespace detail
	// {
	// 	DCORE_HAS_MEMBER(Update);
	// 	DCORE_HAS_MEMBER(Start);
	// 	DCORE_HAS_MEMBER(End);
	// 	DCORE_HAS_MEMBER(Save);
	// 	DCORE_HAS_MEMBER(Load);
	// } // namespace detail

	// template<class S, class U>
	// System GetSystem()
	// {
	// 	return {&S::Start, &S::Update, &S::End, &S::Save, &S::Load, S::ThisComponentName(), typeid(U)};
	// }

	class ECS
	{
		struct ComponentPool
		{
			dstd::DynamicSparseSet Set_;
			ComponentPool(dstd::USize componentSize);

			void *AddComponent(EntityHandle entity, void *component);
			void *GetComponent(EntityHandle entity);
			EntityHandle GetEntity(dstd::USize index);
		};

	public:
		void Initialize();
		void DeInitialize();

		EntityHandle Create();

		struct Message
		{
			dstd::USize Type;
			void *Payload;
		};

		using MessageHandlerFunc = void (*)(void *user, EntityHandle receiver, Message message);
		struct MessageHandler
		{
			void *User;
			MessageHandlerFunc Func;

			inline void operator()(EntityHandle receiver, Message message) const { Func(User, receiver, message); }
		};

		/**
		 * @brief Sends a message `message` to `receiver`.
		 *
		 * @param message The message to send.
		 */
		inline void Send(EntityHandle receiver, Message message) { MessageHandler_(receiver, message); }

		/**
		 * @brief Broadcasts (i.e. sends to everyone) `message`.
		 *
		 * @param message The message to broadcast.
		 */
		inline void Broadcast(Message message)
		{
			// LOG_F(INFO, "Entities count: %llu", UsedEntities_.GetPacked().size());
			for(EntityHandle handle : UsedEntities_.GetPacked())
			{
				LOG_F(INFO, "Sending for %lu", handle);
				Send(handle, message);
			}
		}

		/** Adds a message handler. */
		void AddHandler(MessageHandler &&handler);

		/** Removes a message handler */
		void RemoveHandler(MessageHandler &&handler);

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
			auto typeIndex    = std::type_index(typeid(T));
			const auto &found = ComponentPools_.find(typeIndex);

			if(found == ComponentPools_.end())
			{
				LOG_F(ERROR, "No component pool for type %s", util::Debug::Demangle(typeid(T).name()).c_str());
				return nullptr;
			}

			return (T *)AllComponentPools_[found->second].GetComponent(entity);
		}

		/**
		 * @brief Adds a component to an entity. The component is constructed in-place.
		 * @note The destructor is called during this method.
		 *
		 * @tparam T The type of the component
		 * @tparam Args Arguments that are passed to the constructor of the component.
		 * @param entity The entity that the component will be attached to.
		 * @param args Arguments that are passed to the constructor of the component.
		 * @return The pointer to the attached component.
		 */
		template<typename T, typename... Args>
		T *AddComponent(EntityHandle entity, Args &&...args)
		{
			T obj             = T(std::forward<Args>(args)...);
			auto typeIndex    = std::type_index(typeid(T));
			const auto &found = ComponentPools_.find(typeIndex);

			if(found == ComponentPools_.end())
			{
				LOG_F(ERROR, "No component pool for type %s", util::Debug::Demangle(typeid(T).name()).c_str());
				return nullptr;
			}

			return (T *)AllComponentPools_[found->second].AddComponent(entity, &obj);
		}

		template<typename T>
		void AddComponentPool()
		{
			AllComponentPools_.push_back(ComponentPool(sizeof(T)));
		}

		class ComponentPoolWrapper
		{
			ComponentPool *ComponentPool_;
			friend class EntityIterator;
			friend class ECS;

			ComponentPoolWrapper(ComponentPool *componentPool) : ComponentPool_(componentPool) {}

		public:
			struct EntityIterator
			{
				friend class ComponentPoolWrapper;
				ComponentPoolWrapper &Bound_;
				dstd::USize Index_;

				EntityIterator(ComponentPoolWrapper &bound, dstd::USize index) : Bound_(bound), Index_(index) {}

			public:
				EntityIterator &operator++() { return Index_++, *this; }
				bool operator==(const EntityIterator &other) const { return other.Index_ == this->Index_; }
				bool operator!=(const EntityIterator &other) const { return !(*this == other); }
				EntityHandle operator*() { return Bound_.ComponentPool_->GetEntity(Index_); }
				dstd::USize CurrentIndex() const { return Index_; }
			};

			EntityIterator begin();
			EntityIterator end();
		};

		ComponentPoolWrapper GetComponentPool(dstd::USize index) { return {&AllComponentPools_[index]}; }

		template<typename T>
		ComponentPoolWrapper GetComponentPool()
		{
			auto typeIndex    = std::type_index(typeid(T));
			const auto &found = ComponentPools_.find(typeIndex);

			if(found == ComponentPools_.end())
			{
				LOG_F(ERROR, "No component pool for type %s", util::Debug::Demangle(typeid(T).name()).c_str());
				return {nullptr};
			}

			return GetComponentPool(found->second);
		}

		dstd::USize GetComponentPoolCount() const { return AllComponentPools_.size(); }

		/**
		 * @brief Holds the begin/end methods for the GetEntities() function.
		 */
		class ComponentPoolIterators
		{
			ECS &ECS_;
			ComponentPoolIterators(ECS &ecs) : ECS_(ecs) {}

			friend class ECS;

		public:
			class ComponentPoolIterator
			{
				ComponentPoolIterators &Bound_;
				dstd::USize Index_;
				ComponentPoolIterator(ComponentPoolIterators &bound, dstd::USize index) : Bound_(bound), Index_(index)
				{
				}

				friend class ComponentPoolIterators;

			public:
				ComponentPoolIterator &operator++() { return Index_++, *this; }
				bool operator==(const ComponentPoolIterator &other) const { return other.Index_ == this->Index_; }
				bool operator!=(const ComponentPoolIterator &other) const { return !(*this == other); }
				ComponentPoolWrapper operator*() { return Bound_.ECS_.GetComponentPool(Index_); }
				dstd::USize CurrentIndex() const { return Index_; }
			};

			ComponentPoolIterator begin() { return {*this, 0}; }
			ComponentPoolIterator end() { return {*this, ECS_.GetComponentPoolCount()}; }
		};

		/**
		 * @brief Get the Component Pool By Name object
		 *
		 * @return dstd::USize
		 */
		dstd::USize GetComponentPoolByName(const std::string &name) const;

		/**
		 * @brief Adds a component to an entity. The component is constructed in-place.
		 * @note The destructor is called during this method.
		 *
		 * @tparam Args Arguments that are passed to the constructor of the component.
		 * @param entity The entity that the component will be attached to.
		 * @param args Arguments that are passed to the constructor of the component.
		 * @return The pointer to the attached component.
		 */
		template<typename... Args>
		void AddComponent(EntityHandle entity, dstd::USize pool, Args &&...args)
		{
			// T obj = T(std::forward<Args>(args)...);
			// AllComponentPools_[pool].AddComponent(entity, &obj);
		}

		void SetMessageHandler(MessageHandler handler);

	private:
		std::vector<ComponentPool> AllComponentPools_;
		std::unordered_map<std::string, dstd::USize> ComponentPoolsByName_;
		std::unordered_map<std::type_index, dstd::USize> ComponentPools_;
		std::vector<EntityHandle> AllEntities_;
		dstd::SparseIntegerSet<EntityHandle> UsedEntities_;
		MessageHandler MessageHandler_;
		EntityHandle NextAvailable_;
	};

	class ECSComponentPoolProvider
	{
		struct ComponentPool
		{
			const std::type_info &Type;
			dstd::USize Size;
		};

	public:
		static ECSComponentPoolProvider *Instance();
		template<typename T>
		void AddComponentPool()
		{
			fprintf(stderr, "Add component pool %s, this = %p, count = %lld\n",
			        util::Debug::Demangle(typeid(T).name()).c_str(), this, AllComponentPools_.size());
			AllComponentPools_.push_back(ComponentPool {typeid(T), sizeof(T)});
		}

	private:
		friend class ECS;
		std::vector<ComponentPool> AllComponentPools_;
	};
}; // namespace dcore::world
