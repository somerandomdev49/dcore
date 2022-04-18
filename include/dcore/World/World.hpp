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
#include <type_traits>
#include <unordered_map>

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
		T *GetComponent(const Entity &entity);

		template<typename T>
		void AddComponent(Entity &entity, const T &comp);

		Entity CreateEntity();
		void RegisterUpdate(void (*func)(World *));

		// temp\late<typename ComponentType, typename FunctionType>
		// void Each(FunctionType func);

		terrain::Terrain *GetTerrain() const;

		float GetRenderDistance() const;
		void SetRenderDistance(float newRenderDistance);

		ECS *GetECS() const { return ECSInstance_; }

		/** @deprecated Save externally if needed */
		void Save(data::FileOutput &output);

		/** @deprecated Load externally if needed (see WorldLoader) */
		void Load(const data::FileInput &input);

		/**
		 * @brief Creates the world terrain and destroys previous one (if exists)
		 *
		 * @param heightmap Heightmap to create terrain from.
		 */
		void CreateTerrain(const resource::Resource<terrain::Heightmap> &heightmap);

	private:
		friend class platform::Context;
		friend class launch::Launch;
		friend class WorldLoaderRegistry;
		friend class WorldMessageHandlerProvider;

		void Initialize();
		void DeInitialize();
		void Start();
		void Update();
		void Render(graphics::RendererInterface *render);
		void End();

		void MessageHandler_(EntityHandle handle, ECS::Message message);
		void DispatchMessage_(CommonMessages message, void *data = nullptr);

		std::vector<void (*)(World *)> Updates_;
		terrain::Terrain *Terrain_;
		ECS *ECSInstance_;

		using MessageHandlerFunc = void (*)(World *world, EntityHandle handle, ECS::Message message);
		std::vector<MessageHandlerFunc> Handlers_;

		static constexpr float RENDER_DISTANCE_DEFAULT = 32;
		float RenderDistance_                          = RENDER_DISTANCE_DEFAULT;
	};

	class WorldMessageHandlerProvider
	{
		friend class World;

	public:
		void AddHandler(World::MessageHandlerFunc func);
		static WorldMessageHandlerProvider *Instance();

	private:
		std::vector<World::MessageHandlerFunc> Handlers_;
	};

	namespace detail
	{
		DCORE_HAS_MEMBER(Update);
		DCORE_HAS_MEMBER(Render);
		DCORE_HAS_MEMBER(Start);
		DCORE_HAS_MEMBER(End);
		DCORE_HAS_MEMBER(Save);
		DCORE_HAS_MEMBER(Load);
	}; // namespace detail

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
				ECSComponentPoolProvider::Instance()->AddComponentPool<T>();
				WorldMessageHandlerProvider::Instance()->AddHandler(&HandleMessage);
			}
		} RegStatic_;

	public:
		/**
		 * @brief Called by the World when a message is sent to the component.
		 *
		 * @param message Message received.
		 */
		static void HandleMessage(World *world, EntityHandle handle, ECS::Message message)
		{
			auto comp = world->GetECS()->GetComponent<T>(handle);
			// LOG_F(INFO, "Handle Message [%s] %s", util::Debug::Demangle(typeid(T).name()).c_str(), comp == nullptr ? "Fail" : "Success");
			if(comp == nullptr) return;
			// LOG_F(INFO, "Handle Message:");
			switch(message.Type)
			{
			case(dstd::USize)CommonMessages::UpdateMessage:
				Update(world, handle, comp);
				break;
			case(dstd::USize)CommonMessages::RenderMessage:
				Render(world, handle, (graphics::RendererInterface *)message.Payload, comp);
				break;
			case(dstd::USize)CommonMessages::StartMessage:
				Start(world, handle, comp);
				break;
			case(dstd::USize)CommonMessages::EndMessage:
				End(world, handle, comp);
				break;
			case(dstd::USize)CommonMessages::SaveMessage:
				Save(world, handle, *(data::Json *)message.Payload, comp);
				break;
			case(dstd::USize)CommonMessages::LoadMessage:
				Load(world, handle, *(data::Json *)message.Payload, comp);
				break;
			default: break;
			}
		}

		/**
		 * @brief Returns the name of the component class for the current template instance
		 *
		 * @return The name of the component class if specialized, otherwise the typeinfo name.
		 */
		static std::string ThisComponentName() { return util::Debug::Demangle(typeid(T).name()); }

#define HELPER_(NAME, ...)                                                           \
	template<typename Q = T>                                                         \
	static typename std::enable_if<!detail::has_##NAME<Q>()>::type NAME(__VA_ARGS__) \
	{                                                                                \
	}                                                                                \
                                                                                     \
	template<typename Q = T>                                                         \
	static typename std::enable_if<detail::has_##NAME<Q>()>::type NAME(__VA_ARGS__)

		HELPER_(Start, World *world, EntityHandle self, T *comp)
		{
			comp->Start(self);
		}

		HELPER_(Update, World *world, EntityHandle self, T *comp)
		{
			comp->Update(self);
		}

		HELPER_(Render, World *world, EntityHandle self, graphics::RendererInterface *renderer, T *comp)
		{
			comp->Render(self, renderer);
		}

		HELPER_(End, World *world, EntityHandle self, T *comp)
		{
			comp->End(self);
		}

		HELPER_(Save, World *world, EntityHandle self, data::Json &output, T *comp)
		{
			comp->Save(self, output);
		}

		HELPER_(Load, World *world, EntityHandle self, const data::Json &input, T *comp)
		{
			comp->Load(self, input);
		}
#undef HELPER_
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

		// void Save(EntityHandle self, data::Json &output) const;
		void Render(EntityHandle self, graphics::RendererInterface *render) const;
	};

	/**
	 * @brief Component that includes a static mesh for rendering.
	 */
	struct ModelComponent : ComponentBase<ModelComponent>
	{
		ModelComponent(const resource::Resource<graphics::Model> &model) : Model(model) {}
		resource::Resource<graphics::Model> Model;

		// void Save(EntityHandle self, data::Json &output) const;
		void Render(EntityHandle self, graphics::RendererInterface *render) const;
	};

	/** @deprecated Now World has it's own terrain. */
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
	return World_->GetComponent<T>(*this);
}

template<typename T>
void dcore::world::Entity::AddComponent(const T &comp)
{
	return World_->AddComponent<T>(*this, comp);
}

template<typename T>
T *dcore::world::World::GetComponent(const Entity &entity)
{
	return ECSInstance_->GetComponent<T>(entity.Id_);
}

template<typename T>
void dcore::world::World::AddComponent(Entity &entity, const T &comp)
{
	ECSInstance_->AddComponent<T>(entity.Id_, comp);
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
