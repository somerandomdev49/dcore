#pragma once
#include <dcore/Resource/ResourceManager.hpp>
#include <dcore/Misc/Terrain/Terrain.hpp>
#include <dcore/Graphics/Graphics.hpp>
#include <dcore/Data/FileOutput.hpp>
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

		static void Save(data::FileOutput &output, void *comp);
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
		/** void SaveFunction(data::FileOutput &out, ); */
		using SaveFunction = void (*)(data::FileOutput&, void*);

		template<typename T>
		T &GetComponent(const Entity *entity);

		template<typename T>
		void AddComponent(Entity *entity, const T &c);

		Entity CreateEntity();
		void RegisterUpdate(void (*f)(World *));

		template<typename ComponentType, typename FunctionType>
		void Each(FunctionType func);

		const terrain::Terrain &GetTerrain() const;

		float GetRenderDistance() const;
		void SetRenderDistance(float newRenderDistance);

		void Save(data::FileOutput &output);

		template<typename T>
		void RegisterSaveFunction(SaveFunction func);

	private:
		friend class WorldUpdateInfo;
		friend class platform::Context;
		friend class launch::Launch;
		void Initialize();
		void DeInitialize();
		void Update();
		void Render(graphics::RendererInterface *render);
		std::vector<void (*)(World *)> Updates_;
		entt::registry Registry_;
		terrain::Terrain Terrain_;
		float RenderDistance_ = 32.0f;
		std::unordered_map<entt::id_type, SaveFunction> SaveFunctions_;
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

template<typename ComponentType, typename FunctionType>
void dcore::world::World::Each(FunctionType f)
{
	auto view = Registry_.view<ComponentType>();
	view.each(
	    [&](auto entityid, auto &c)
	    {
		    Entity entity(entityid, this);
		    f(&entity, &c);
	    });
}

template<typename T>
void dcore::world::World::RegisterSaveFunction(SaveFunction func)
{
	SaveFunctions_[entt::type_hash<T>::value()] = func;
}
