#pragma once
#include <dcore/Resource/ResourceManager.hpp>
#include <dcore/Misc/Terrain/Terrain.hpp>
#include <dcore/Graphics/Graphics.hpp>
#include <entt/entity/registry.hpp>
#include <dcore/Launch.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace dcore::platform
{
	class Context;
}

namespace dcore::world
{
	struct TransformComponent
	{
		entt::entity Parent;
		glm::vec3 Position;
		glm::quat Rotation;
		glm::vec3 Scale;

		glm::mat4 Matrix;

		/** Recalculates the transform matrix. */
		void ReCalculateMatrix();
	};

	struct StaticMeshComponent
	{
		graphics::StaticMesh Mesh;
	};

	class World;
	struct DynamicComponent
	{
		void *Data;
		void (*Update)(void *, World *);
	};

	class Entity
	{
	public:
		Entity(entt::entity id, World *world);

		template<typename T>
		void AddComponent(const T &c);

		template<typename T>
		T &GetComponent() const;

		/** Returns the entity's id. */
		entt::entity GetId() const;

	private:
		friend class World;
		entt::entity Id_;
		World *World_;
	};

	class World
	{
	public:
		// class WorldUpdateInfo
		// {
		// public:
		//     template<typename T>
		//     void Each(void (*func)(Entity *entity, T *component));
		// private:
		//     friend class World;
		//     World *World_;
		// };

		template<typename T>
		T &GetComponent(const Entity *entity);

		template<typename T>
		void AddComponent(Entity *entity, const T &c);

		Entity CreateEntity();
		void RegisterUpdate(void (*f)(World *));

		template<typename ComponentType>
		void Each(void (*func)(Entity *entity, ComponentType *component));

	private:
		friend class WorldUpdateInfo;
		friend class platform::Context;
		friend class launch::Launch;
		void Initialize();
		void DeInitialize();
		void Update();
		void Render(graphics::RendererInterface *render);
		std::vector<void (*)(World *)> Updates_; // TODO: is std::vector too much?
		entt::registry Registry_;
		terrain::Terrain Terrain_;
	};
} // namespace dcore::world

template<typename T>
T &dcore::world::Entity::GetComponent() const
{
	return World_->GetComponent<T>(this);
}

template<typename T>
void dcore::world::Entity::AddComponent(const T &c)
{
	return World_->AddComponent<T>(this, c);
}

template<typename T>
T &dcore::world::World::GetComponent(const Entity *entity)
{
	return Registry_.get<T>(entity->Id_);
}

template<typename T>
void dcore::world::World::AddComponent(Entity *entity, const T &c)
{
	Registry_.emplace<T>(entity->Id_, c);
}

template<typename ComponentType>
void dcore::world::World::Each(void (*func)(Entity *entity, ComponentType *component))
{
	auto view = Registry_.view<std::decay_t<ComponentType>>();
	// view.each([&](const entt::entity &entityid, auto &c){
	//     Entity entity(entityid, World_);
	//     func(&entity, &c);
	// });
	for(auto e : view)
	{
		auto &c = view.template get<ComponentType>(e); // std::decay_t<ComponentType>
		Entity en(e, this);
		func(&en, &c);
	}
}
