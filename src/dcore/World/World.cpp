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
	DCORE_COMPONENT_REGISTER(ModelComponent);
	DCORE_COMPONENT_REGISTER(DynamicComponent);
	DCORE_COMPONENT_REGISTER(TerrainComponent);

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

	void ModelComponent::Save(const EntityHandle &self, data::Json &output)
	{
		(void)self;
		output = data::Json::object({{"model", Model.GetName()}});
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

	TerrainComponent::TerrainComponent(const resource::Resource<terrain::Heightmap> &heightmap)
	{
		Terrain_.Initialize(resource::GetResource<terrain::Heightmap>("DCore.Heightmap.World1"));
		Terrain_.ActivateAllChunks();
		platform::Context::Instance()->GetWorld()->SetTerrain(&Terrain_);
	}

	void TerrainComponent::Update(const EntityHandle &self)
	{
		Terrain_.ReactivateChunks(platform::Context::Instance()->GetRendererInterface()->GetCamera()->GetPosition(),
		                          platform::Context::Instance()->GetWorld()->GetRenderDistance());
	}

	void World::Initialize() { RenderDistance_ = Preferences::Instance()->GetGraphicsSettings().RenderDistance; }

	terrain::Terrain &TerrainComponent::GetTerrain() { return Terrain_; }

	terrain::Terrain *World::GetTerrain() const { return Terrain_; }
	void World::SetTerrain(terrain::Terrain *terrain) { Terrain_ = terrain; }

	void World::DeInitialize() { ECSInstance()->DeInitialize(); }

	void World::Update()
	{
		// TODO: Handle UI clicks and other events.
		// for(auto it = ECSInstance()->begin(); it != ECSInstance()->end(); ++it)
		// {
		// 	const auto &systems = ECSInstance()->GetSystems(*it);
		// 	for(const auto &system : systems) system->UpdateFunction(*it);
		// }

		for(const auto &system : ECSInstance()->GetAllSystems())
		{
			LOG_F(INFO, "%s <- register", util::Debug::Demangle(system.Type.name()).c_str());
			auto entities = ECSInstance()->GetEntities(system.Type);
			for(const auto &entity : entities)
			{
				system.UpdateFunction(*entity);
			}
		}
	}

	static dcore::resource::Resource<dcore::graphics::RStaticMesh> cubeMesh__;
	void World::Start()
	{
		// LOG_F(ERROR, "begin: %ld, end: %ld", ECSInstance()->begin().CurrentIndex(),
		//         ECSInstance()->end().CurrentIndex());
		for(auto it = ECSInstance()->begin(); it != ECSInstance()->end(); ++it)
		{
			// FUUUCK we have a giant leak somewhere?
			if(it.CurrentIndex() > ECSInstance()->end().CurrentIndex()) break;
			fprintf(stderr, "it: %lu, end: %lu\n", it.CurrentIndex(), ECSInstance()->end().CurrentIndex());
			const auto &systems = ECSInstance()->GetSystems(*it);
			for(const auto &system : systems) system->StartFunction(*it);
			break;
		}

		cubeMesh__ = dcore::resource::GetResource<graphics::RStaticMesh>("DCore.Mesh.Cube");
		graphics::gui::GuiManager::Instance()->InitializeRoot_();
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

		// Render StaticMeshComponent entities.
		{
			auto entities = ECSInstance()->GetEntities<StaticMeshComponent>();
			for(const auto &entity : entities)
			{
				// fprintf(stderr, "bryh\n");
				auto transform  = ECSInstance()->GetComponent<TransformComponent>(*entity);
				auto staticMesh = ECSInstance()->GetComponent<StaticMeshComponent>(*entity);

				// if(transform.IsDirty()) transform.ReCalculateMatrix();
				staticMesh->Mesh.SetTransform(transform->GetMatrix());

				render->RenderStaticMesh(&staticMesh->Mesh);
			}
		}

		// Render ModelComponent entities.
		{
			auto entities = ECSInstance()->GetEntities<ModelComponent>();
			for(const auto &entity : entities)
			{
				auto transform = ECSInstance()->GetComponent<TransformComponent>(*entity);
				auto model     = ECSInstance()->GetComponent<ModelComponent>(*entity);

				// if(transform.IsDirty()) transform.ReCalculateMatrix();

				render->RenderModel(model->Model.Get(), transform->GetMatrix());
			}
		}

		// Render Terrain if it exists.
		LOG_F(INFO, "Maybe will render terrain");
		if(Terrain_)
		{
			LOG_F(INFO, "Rendering terrain:");
			auto &chunks = Terrain_->GetChunks();
			for(auto ci : Terrain_->GetActiveChunks()) render->RenderChunk(&chunks[ci]);

			// const auto &entities = ECSInstance()->GetEntities<TerrainComponent>();
			// for(const auto &entity : entities)
			// {
			// 	auto terrain = ECSInstance()->GetComponent<TerrainComponent>(entity);
			// 	if(terrain == nullptr) return; // FIXME: Temporary
			// 	auto &chunks = terrain->GetTerrain().GetChunks();
			// }
		}
		LOG_F(INFO, "Done rendering");

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

	void World::Load(const data::FileInput &input)
	{
		if(input.Get()["version"] != "0.01")
		{
			DCORE_LOG_ERROR << "Incorrect savefile version: '" << input.Get()["version"] << "', errors may occur.";
			// TODO: Create backup here.
		}

		const auto &entities  = input.Get()["entities"];
		unsigned int entityNo = 0;
		for(const auto &ej : entities)
		{
			EntityHandle id = ECSInstance()->CreateEntity();

			if(!ej.contains("uuid"))
			{
				LOG_F(WARNING, "Loading entity without a UUID! (Entity #%u)", entityNo);
			}
			else
			{
				auto uuidJson = ej["uuid"].get<std::string>();
				dstd::UUID uuid;
				dstd::UUID::Parse(uuid, uuidJson);
				ECSInstance()->AddComponent(id, UUIDComponent(std::move(uuid)));
			}

			for(const auto &comp : ej["components"])
			{
				const auto &system = ECSInstance()->GetSystemByName(comp["@type"]);
				ECSInstance()->AddEntityToSystem(system, id);
			}

			entityNo += 1;
		}
	}
} // namespace dcore::world
