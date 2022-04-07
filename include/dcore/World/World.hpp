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

	enum class CommonMessages : dstd::USize
	{
		StartMessage,
		EndMessage,
		UpdateMessage,
		RenderMessage,
		PreRenderMessage,
		PostRenderMessage,
		SaveMessage,
		LoadMessage,
	};

	/**
	 * @brief Wrapper for @ref dcore::world::ECS and terrain
	 */
	class World
	{
	public:
		/** void SaveFunction(data::FileOutput &out, ); */
		/** @deprecated Save externally */
		using SaveFunction = void (*)(data::FileOutput &, void *);

		template<typename T>
		T *GetComponent(const Entity *entity);

		template<typename T>
		void AddComponent(Entity *entity, const T &comp);

		Entity CreateEntity();
		void RegisterUpdate(void (*func)(World *));

		// template<typename ComponentType, typename FunctionType>
		// void Each(FunctionType func);

		terrain::Terrain *GetTerrain() const;
		void SetTerrain(terrain::Terrain *terrain);

		float GetRenderDistance() const;
		void SetRenderDistance(float newRenderDistance);

		/** @deprecated Save externally if needed */
		void Save(data::FileOutput &output);
		/** @deprecated Load externally if needed (see WorldLoader) */
		void Load(const data::FileInput &input);

	private:
		friend class platform::Context;
		friend class launch::Launch;
		friend class WorldLoaderRegistry;
		void Initialize();
		void DeInitialize();
		void Start();
		void Update();
		void Render(graphics::RendererInterface *render);
		void End();

		void DispatchMessage_(CommonMessages message, void *data = nullptr);

		std::vector<void (*)(World *)> Updates_;
		terrain::Terrain *Terrain_;
		ECS *ECSInstance_;

		static constexpr float RENDER_DISTANCE_DEFAULT = 32;
		float RenderDistance_                          = RENDER_DISTANCE_DEFAULT;
	};

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
			Reg() { ECSComponentPoolProvider::Instance()->AddComponentPool<T>(); }
		} RegStatic_;

	public:
		/**
		 * @brief Returns the name of the component class for the current template instance
		 *
		 * @return The name of the component class if specialized, otherwise the typeinfo name.
		 */
		static std::string ThisComponentName() { return util::Debug::Demangle(typeid(T).name()); }

		// static void Start(const EntityHandle &self)
		// {
		// 	// Do not run if the Component type doesn't implement Start.
		// 	if constexpr(!detail::has_Start<T>()) return;
		// 	T *comp = ECSInstance()->GetComponent<T>(self);
		// 	// No need for NULL checking here, if this is called, the component exists.
		// 	comp->Start(self);
		// }

		// static void Update(const EntityHandle &self)
		// {
		// 	if constexpr(!detail::has_Update<T>()) return;
		// 	T *comp = ECSInstance()->GetComponent<T>(self);
		// 	comp->Update(self);
		// }

		// static void End(const EntityHandle &self)
		// {
		// 	if constexpr(!detail::has_End<T>()) return;
		// 	T *comp = ECSInstance()->GetComponent<T>(self);
		// 	comp->End(self);
		// }

		// static void Save(const EntityHandle &self, data::Json &output)
		// {
		// 	if constexpr(!detail::has_Save<T>()) return;
		// 	T *comp = ECSInstance()->GetComponent<T>(self);
		// 	comp->Save(self, output);
		// }

		// static void Load(const EntityHandle &self, const data::Json &input)
		// {
		// 	if constexpr(!detail::has_Load<T>()) return;
		// 	T *comp = ECSInstance()->GetComponent<T>(self);
		// 	comp->Load(self, input);
		// }
	};

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

		void Save(const EntityHandle &entity, data::Json &output) const;

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

		void Save(const EntityHandle &self, data::Json &output) const;
	};

	/**
	 * @brief Component that includes a static mesh for rendering.
	 */
	struct ModelComponent : ComponentBase<ModelComponent>
	{
		ModelComponent(const resource::Resource<graphics::Model> &model) : Model(model) {}
		resource::Resource<graphics::Model> Model;

		void Save(const EntityHandle &self, data::Json &output) const;
	};

	class TerrainComponent : public ComponentBase<TerrainComponent>
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
		Entity(EntityHandle handle, World *world);

		template<typename T>
		void AddComponent(const T &comp);

		template<typename T>
		T *GetComponent() const;

		/** Returns the entity's id. */
		EntityHandle GetId() const;

	private:
		friend class World;
		EntityHandle Id_;
		World *World_;
	};

	struct UUIDComponent : ComponentBase<UUIDComponent>
	{
		dstd::UUID Value;

		UUIDComponent(dstd::UUID uuid) : Value(uuid) {}
	};
} // namespace dcore::world

template<typename T>
T *dcore::world::Entity::GetComponent() const
{
	return World_->GetComponent<T>(this);
}

template<typename T>
void dcore::world::Entity::AddComponent(const T &comp)
{
	return World_->AddComponent<T>(this, comp);
}

template<typename T>
T *dcore::world::World::GetComponent(const Entity *entity)
{
	return ECSInstance_->GetComponent<T>(entity->Id_);
}

template<typename T>
void dcore::world::World::AddComponent(Entity *entity, const T &comp)
{
	ECSInstance_->AddComponent<T>(entity->Id_, comp);
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
