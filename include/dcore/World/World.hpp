#pragma once
#include <dcore/Core/Refl.hpp>
#include <dcore/Resource/ResourceManager.hpp>
#include <dcore/Misc/Terrain/Terrain.hpp>
#include <dcore/Graphics/Graphics.hpp>
#include <dcore/Data/FileOutput.hpp>
#include <dcore/Data/FileInput.hpp>
// #include <entt/entity/registry.hpp>
#include <dcore/World/Entity.hpp>
#include <dcore/Launch.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace dcore::platform
{
	class Context;
}

namespace dcore::world
{
	class Entity; // forward declaration.

	/**
	 * @brief A component that contains positional and hierarchical data for an entity.
	 */
	struct TransformComponent : ComponentBase<TransformComponent>
	{
	public:
		Entity *GetParent() const;
		const glm::vec3 &GetPosition() const;
		const glm::quat &GetRotation() const;
		const glm::vec3 &GetScale() const;
		const glm::mat4 &GetMatrix() const;
		const glm::mat4 &GetMatrix();

		void SetPosition(const glm::vec3 &newPosition);
		void SetRotation(const glm::quat &newRotation);
		void SetScale(const glm::vec3 &newScale);

		bool IsDirty() const;

		/** Recalculates the transform matrix. */
		void ReCalculateMatrix();

		void Save(const EntityHandle &entity, data::Json &output);

	private:
		glm::mat4 GetNewMatrix() const;

		Entity *Parent_     = nullptr;
		glm::vec3 Position_ = glm::vec3(0);
		glm::quat Rotation_ = glm::identity<glm::quat>();
		glm::vec3 Scale_    = glm::vec3(1);
		glm::mat4 Matrix_   = glm::mat4(1);
		bool Dirty_         = false;
	};

	/**
	 * @brief Component that includes a static mesh for rendering.
	 */
	struct StaticMeshComponent : ComponentBase<StaticMeshComponent>
	{
		StaticMeshComponent(const graphics::StaticMesh &mesh) : Mesh(mesh) {}
		graphics::StaticMesh Mesh;

		void Save(const EntityHandle &entity, data::Json &output);
	};

	/**
	 * @brief Component that includes a static mesh for rendering.
	 */
	struct ModelComponent : ComponentBase<ModelComponent>
	{
		ModelComponent(const resource::Resource<graphics::Model> &model) : Model(model) {}
		resource::Resource<graphics::Model> Model;

		void Save(const EntityHandle &entity, data::Json &output);
	};

	class TerrainComponent : ComponentBase<TerrainComponent>
	{
	public:
		TerrainComponent(const resource::Resource<terrain::Heightmap> &heightmap);

		terrain::Terrain &GetTerrain();
		const terrain::Terrain &GetTerrain() const;

		void Update(const EntityHandle &self);

	private:
		terrain::Terrain Terrain_;
	};

	class World;

	/**
	 * @brief Component for scripting, currently deprecated.
	 * @deprecated
	 */
	struct DynamicComponent : ComponentBase<DynamicComponent>
	{
		void *Data;
		// void (*Update)(void *, World *);
	};

	/**
	 * @brief Wrapper for @ref dcore::world::EntityHandle
	 */
	class Entity
	{
	public:
		Entity(EntityHandle id, World *world);

		template<typename T>
		void AddComponent(const T &c);

		template<typename T>
		T *GetComponent() const;

		/** Returns the entity's id. */
		EntityHandle GetId() const;

	private:
		friend class World;
		EntityHandle Id_;
		World *World_;
	};

	/**
	 * @brief Wrapper for @ref dcore::world::ECS and terrain
	 */
	class World
	{
	public:
		/** void SaveFunction(data::FileOutput &out, ); */
		using SaveFunction = void (*)(data::FileOutput &, void *);

		template<typename T>
		T *GetComponent(const Entity *entity);

		template<typename T>
		void AddComponent(Entity *entity, const T &c);

		Entity CreateEntity();
		void RegisterUpdate(void (*f)(World *));

		// template<typename ComponentType, typename FunctionType>
		// void Each(FunctionType func);

		terrain::Terrain *GetTerrain() const;
		void SetTerrain(terrain::Terrain *terrain);

		float GetRenderDistance() const;
		void SetRenderDistance(float newRenderDistance);

		void Save(data::FileOutput &output);
		void Load(const data::FileInput &input);

	private:
		friend class platform::Context;
		friend class launch::Launch;
		void Initialize();
		void DeInitialize();
		void Start();
		void Update();
		void Render(graphics::RendererInterface *render);
		void End();

		std::vector<void (*)(World *)> Updates_;
		terrain::Terrain *Terrain_;
		float RenderDistance_ = 32.0f;
	};
} // namespace dcore::world

template<typename T>
T *dcore::world::Entity::GetComponent() const
{
	return World_->GetComponent<T>(this);
}

template<typename T>
void dcore::world::Entity::AddComponent(const T &c)
{
	return World_->AddComponent<T>(this, c);
}

template<typename T>
T *dcore::world::World::GetComponent(const Entity *entity)
{
	return ECSInstance()->GetComponent<T>(entity->Id_);
}

template<typename T>
void dcore::world::World::AddComponent(Entity *entity, const T &c)
{
	ECSInstance()->AddComponent<T>(entity->Id_, c);
}

// template<typename ComponentType, typename FunctionType>
// void dcore::world::World::Each(FunctionType f)
// {
// 	auto view = Registry_.view<ComponentType>();
// 	view.each(
// 	    [&](auto entityid, auto &c)
// 	    {
// 		    Entity entity(entityid, this);
// 		    f(&entity, &c);
// 	    });
// }

// template<typename T>
// void dcore::world::World::RegisterSaveFunction(SaveFunction func)
// {
// 	SaveFunctions_[std::type_index(typeid(func))] = func;
// }
