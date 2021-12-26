#include <dcore/World/World.hpp>
#include <dcore/Graphics/GUI/GuiGraphics.hpp>
#include <dcore/Graphics/GUI/GuiManager.hpp>
#include <dcore/Util/JsonConverters.hpp>
#include <dcore/Graphics/GUI/Font.hpp>
#include <dcore/Graphics/Graphics.hpp>
#include <dcore/Platform/Platform.hpp>
#include <dcore/Core/Preferences.hpp>
#include <iostream>

namespace dcore::world
{
	DCORE_COMPONENT_REGISTER(TransformComponent);
	DCORE_COMPONENT_REGISTER(StaticMeshComponent);
	DCORE_COMPONENT_REGISTER(DynamicComponent);

	void StaticMeshComponent::Save(const EntityHandle &self, data::Json &output)
	{
		(void)self;
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
		(void)self;
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
		RenderDistance_ = Preferences::Instance()->GetGraphicsSettings().RenderDistance;
	}

	const terrain::Terrain &World::GetTerrain() const { return Terrain_; }

	void World::DeInitialize() {}

	void World::Update()
	{
		// TODO: Handle UI clicks and other events.
		for(const auto &entity : *ECSInstance())
		{
			const auto &systems = ECSInstance()->GetSystems(entity);
			for(const auto &system : systems) system->UpdateFunction(entity);
		}
	}

	static dcore::resource::Resource<dcore::graphics::RStaticMesh> cubeMesh__;
	void World::Start()
	{
		for(const auto &entity : *ECSInstance())
		{
			const auto &systems = ECSInstance()->GetSystems(entity);
			for(const auto &system : systems) system->StartFunction(entity);
		}

		cubeMesh__ = dcore::resource::ResourceManager::Instance()->Get<graphics::RStaticMesh>("DCore.Mesh.Cube");
	}

	void World::End()
	{
		for(const auto &entity : *ECSInstance())
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

		platform::Context::Instance()->GetRendererInterface()->GetRenderer()->DisableDepthCheck();

		// Render the UI. (Maybe this doesn't belong here...)
		graphics::gui::GuiManager::Instance()->Render(graphics::gui::GuiGraphics::Instance());

		platform::Context::Instance()->GetRendererInterface()->GetRenderer()->EnableDepthCheck();
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

		for(const auto &entity : *ECSInstance())
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

	void World::Load(data::FileInput &input)
	{
		if(input.Get()["version"] != "0.01")
		{
			DCORE_LOG_ERROR << "Incorrect savefile version: '" << input.Get()["version"] << "', errors may occur.";
			// TODO: Create backup here.
		}

		const auto &entities = input.Get()["entities"];
		for(const auto &ej : entities)
		{
			EntityHandle id = ej["id"].get<std::decay_t<EntityHandle>>();
			ECSInstance()->CreateEntityWithId(id);

			for(const auto &comp : ej["components"])
			{
				const auto &system = ECSInstance()->GetSystemByName(comp["@type"]);
				ECSInstance()->AddEntityToSystem(system, id);
			}

			// data::Json comps = data::Json::array();

			// const auto &systems = ECSInstance()->GetSystems(entity);
			// for(const auto &system : systems)
			// {
			// 	data::Json out = data::Json::object();
			// 	system->SaveFunction(entity, out);

			// 	out["@type"] = system->Name;
			// 	comps.push_back(out);
			// }

			// output.Get()["entities"].push_back(data::Json {{"id", entity}, {"components", comps}});
		}
	}
} // namespace dcore::world
