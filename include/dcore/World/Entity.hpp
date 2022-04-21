#pragma once
#include "entt/entity/fwd.hpp"
#include <dcore/Core/DynamicSparseDataSet.hpp>
#include <dcore/Util/TypeTraits.hpp>
#include <dcore/Data/FileOutput.hpp>
#include <dcore/Core/SparseSet.hpp>
#include <dcore/Util/Debug.hpp>
#include <dcore/Core/UUID.hpp>
#include <dcore/Core/Type.hpp>
#include <dcore/Core/Log.hpp>
#include <dcore/Uni.hpp>
#include <entt/entity/registry.hpp>
#include <typeindex>
#include <typeinfo>
#include <cstdio>

namespace dcore::world
{
	// If the first bit is 1 then the entity is considered dead, alive otherwise.
	using EntityHandle = entt::entity;

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
		void Broadcast(Message message);

		/** Adds a message handler. */
		void AddHandler(MessageHandler &&handler);

		/** Removes a message handler */
		void RemoveHandler(MessageHandler &&handler);

		template<typename ...Components>
		void Each(auto func) {
			Registry_.view<Components...>().each(func);
		}

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
			return Registry_.try_get<T>(entity);
		}

		/**
		 * @brief Adds a component to an entity. The component is constructed in-place.
		 *
		 * @tparam Args Arguments that are passed to the constructor of the component.
		 * @param entity The entity that the component will be attached to.
		 * @param args Arguments that are passed to the constructor of the component.
		 * @return The pointer to the attached component.
		 */
		template<typename T, typename... Args>
		void AddComponent(EntityHandle entity, Args &&...args)
		{
			Registry_.emplace<T>(entity, std::forward<Args>(args)...);
		}

		void SetMessageHandler(MessageHandler handler);

	private:
		using Registry = entt::basic_registry<entt::entity>;
		std::vector<EntityHandle> AllEntities_;
		Registry Registry_;
		// std::vector<ComponentPool> AllComponentPools_;
		// std::unordered_map<std::string, dstd::USize> ComponentPoolsByName_;
		// std::unordered_map<std::type_index, dstd::USize> ComponentPools_;
		// dstd::SparseIntegerSet<EntityHandle> UsedEntities_;
		MessageHandler MessageHandler_;
		// EntityHandle NextAvailable_;
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
