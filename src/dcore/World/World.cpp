#include <dcore/World/World.hpp>
#include <dcore/Graphics/GUI/GuiGraphics.hpp>
#include <dcore/Graphics/GUI/GuiManager.hpp>
#include <dcore/Util/JsonConverters.hpp>
#include <dcore/Graphics/GUI/Font.hpp>
#include <dcore/Graphics/Graphics.hpp>
#include <iostream>

namespace dcore::world
{

	DCORE_COMPONENT_REGISTER(TransformComponent);
	DCORE_COMPONENT_AUTO_NAME(TransformComponent);

	DCORE_COMPONENT_REGISTER(StaticMeshComponent);
	DCORE_COMPONENT_AUTO_NAME(StaticMeshComponent);

	DCORE_COMPONENT_REGISTER(DynamicComponent);
	DCORE_COMPONENT_AUTO_NAME(DynamicComponent);

	void StaticMeshComponent::Save(const EntityHandle &self, data::Json &output)
	{
		std::string name = Mesh.GetTexture().GetName();
		std::cout << "Saving StaticMeshComponent, GetTexture().GetName() = " << name << std::endl;
		auto meshInfo = data::Json {
		    {"mesh", Mesh.GetMesh().GetName()},
		    {"texture", Mesh.GetTexture().GetName()},
		};

		output = data::Json {{"mesh", meshInfo}};
	}

	void TransformComponent::Save(const EntityHandle &self, data::Json &output)
	{
		output = data::Json::object({
		    {"position", util::JsonConverters::Glm(this->Position_)},
		    {"rotation", util::JsonConverters::Glm(this->Rotation_)},
		    {"scale", util::JsonConverters::Glm(this->Scale_)},
		});
	}

	void TransformComponent::ReCalculateMatrix()
	{
		Matrix_ = GetNewMatrix();
		Dirty_  = false;
	}

	bool TransformComponent::IsDirty() const { return Dirty_; }

	glm::mat4 TransformComponent::GetNewMatrix() const
	{
		glm::mat4 rot = glm::mat4_cast(Rotation_);
		glm::mat4 pos = glm::translate(glm::mat4(1.0f), Position_);
		glm::mat4 scl = glm::scale(glm::mat4(1.0f), Scale_);
		return scl * pos * rot;
	}

	Entity *TransformComponent::GetParent() const { return Parent_; }
	const glm::vec3 &TransformComponent::GetPosition() const { return Position_; }
	const glm::quat &TransformComponent::GetRotation() const { return Rotation_; }
	const glm::vec3 &TransformComponent::GetScale() const { return Scale_; }
	const glm::mat4 &TransformComponent::GetMatrix()
	{
		if(Dirty_) ReCalculateMatrix();
		return Matrix_;
	}

	const glm::mat4 &TransformComponent::GetMatrix() const
	{
		static glm::mat4 newMatrixTemp; // Hacky solution but we need to return a reference!
		if(Dirty_) return newMatrixTemp = GetNewMatrix();
		return Matrix_;
	}

	void TransformComponent::SetPosition(const glm::vec3 &newPosition)
	{
		Position_ = newPosition;
		Dirty_    = true;
	}

	void TransformComponent::SetRotation(const glm::quat &newRotation)
	{
		Rotation_ = newRotation;
		Dirty_    = true;
	}

	void TransformComponent::SetScale(const glm::vec3 &newScale)
	{
		Scale_ = newScale;
		Dirty_ = true;
	}

	void World::Initialize()
	{
		Terrain_.Initialize(resource::ResourceManager::Instance()->Get<terrain::Heightmap>("DCore.Heightmap.World1"));
		Terrain_.ActivateAllChunks();
	}

	void World::DeInitialize() {}

	void World::Update()
	{
		// TODO: Handle UI clicks and other events.
		const auto &entities = ECSInstance()->GetAllEntities();
		for(const auto &entity : entities)
		{
			const auto &systems = ECSInstance()->GetSystems(entity);
			for(const auto &system : systems) system->UpdateFunction(entity);
		}
	}

	static dcore::resource::Resource<dcore::graphics::RStaticMesh> cubeMesh__;
	void World::Start()
	{
		const auto &entities = ECSInstance()->GetAllEntities();
		for(const auto &entity : entities)
		{
			const auto &systems = ECSInstance()->GetSystems(entity);
			for(const auto &system : systems) system->StartFunction(entity);
		}

		cubeMesh__ = dcore::resource::ResourceManager::Instance()->Get<graphics::RStaticMesh>("DCore.Mesh.Cube");
	}

	void World::End()
	{
		const auto &entities = ECSInstance()->GetAllEntities();
		for(const auto &entity : entities)
		{
			const auto &systems = ECSInstance()->GetSystems(entity);
			for(const auto &system : systems) system->EndFunction(entity);
		}
	}

	void World::Render(graphics::RendererInterface *render)
	{
		Terrain_.ReactivateChunks(render->GetCamera()->GetPosition(), RenderDistance_);

		const auto &entities = ECSInstance()->GetEntities<StaticMeshComponent>();
		for(const auto &entity : entities)
		{
			auto &transform  = ECSInstance()->GetComponent<TransformComponent>(entity);
			auto &staticMesh = ECSInstance()->GetComponent<StaticMeshComponent>(entity);

			// if(transform.IsDirty()) transform.ReCalculateMatrix();
			staticMesh.Mesh.SetTransform(transform.GetMatrix());

			render->RenderStaticMesh(&staticMesh.Mesh);
		}

		auto &chunks = Terrain_.GetChunks();
		for(auto ci : Terrain_.GetActiveChunks()) render->RenderChunk(&chunks[ci]);

		// Render the UI. (Maybe this doesn't belong here...)
		graphics::gui::GuiManager::Instance()->Render(graphics::gui::GuiGraphics::Instance());
	}

	dcore::world::EntityHandle Entity::GetId() const { return Id_; }
	Entity::Entity(dcore::world::EntityHandle e, World *world) : Id_(e), World_(world) {}

	Entity World::CreateEntity() { return Entity(ECSInstance()->CreateEntity(), this); }

	void World::RegisterUpdate(void (*f)(World *)) { Updates_.push_back(f); }

	float World::GetRenderDistance() const { return RenderDistance_; }
	void World::SetRenderDistance(float newRenderDistance) { RenderDistance_ = newRenderDistance; }

	void World::Save(data::FileOutput &output)
	{
		output.Get() = data::Json {{"version", "0.01"}, {"entities", data::Json::array()}};

		const auto &entities = ECSInstance()->GetAllEntities();
		for(const auto &entity : entities)
		{
			data::Json comps = data::Json::array();

			const auto &systems = ECSInstance()->GetSystems(entity);
			for(const auto &system : systems)
			{
				data::Json out = data::Json::object();
				system->SaveFunction(entity, out);

				out["@type"] = system->Name;
				comps.push_back(out);
			}

			output.Get()["entities"].push_back(data::Json {{"id", entity}, {"components", comps}});
		}
	}
} // namespace dcore::world
