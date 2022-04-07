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

	void StaticMeshComponent::Save(const EntityHandle &self, data::Json &output) const
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

	void TransformComponent::Save(const EntityHandle &entity, data::Json &output) const
	{
		(void)entity;
		output = data::Json::object({
		    {"position", util::JsonConverters::Glm(this->Position_)},
		    {"rotation", util::JsonConverters::Glm(this->Rotation_)},
		    {"scale", util::JsonConverters::Glm(this->Scale_)},
		});
	}

	void ModelComponent::Save(const EntityHandle &self, data::Json &output) const
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

	void World::Initialize()
	{
		RenderDistance_ = Preferences::Instance()->GetGraphicsSettings().RenderDistance;
		ECSInstance_ = new ECS();
		ECSInstance_->Initialize();

		LOG_F(WARNING, "World::Initialize??");
	}

	void World::DeInitialize()
	{
		ECSInstance_->DeInitialize();
		delete ECSInstance_;
	}

	terrain::Terrain &TerrainComponent::GetTerrain() { return Terrain_; }

	terrain::Terrain *World::GetTerrain() const { return Terrain_; }
	void World::SetTerrain(terrain::Terrain *terrain) { Terrain_ = terrain; }

	void World::DispatchMessage_(CommonMessages message, void *data)
	{
		ECSInstance_->Broadcast({ (dstd::USize)message, data });
	}

	void World::Update()
	{
		// TODO: Handle UI clicks and other events.
		DispatchMessage_(CommonMessages::UpdateMessage);
	}

	static dcore::resource::Resource<dcore::graphics::RStaticMesh> cubeMesh__;
	void World::Start()
	{
		DispatchMessage_(CommonMessages::StartMessage);
		cubeMesh__ = dcore::resource::GetResource<graphics::RStaticMesh>("DCore.Mesh.Cube");
		graphics::gui::GuiManager::Instance()->InitializeRoot_();
	}

	void World::End() { DispatchMessage_(CommonMessages::EndMessage); }

	void World::Render(graphics::RendererInterface *render)
	{
		DispatchMessage_(CommonMessages::RenderMessage, render);

		// Render Terrain if it exists.
		LOG_F(INFO, "Maybe will render terrain");
		if(Terrain_ != nullptr)
		{
			LOG_F(INFO, "Rendering terrain:");
			const auto &chunks = Terrain_->GetChunks();
			for(auto chunkIndex : Terrain_->GetActiveChunks())
				render->RenderChunk(&chunks[chunkIndex]);

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

	EntityHandle Entity::GetId() const { return Id_; }
	Entity::Entity(EntityHandle handle, World *world) : Id_(handle), World_(world) {}

	Entity World::CreateEntity() { return {ECSInstance_->Create(), this}; }

	void World::RegisterUpdate(void (*func)(World *)) { Updates_.push_back(func); }

	float World::GetRenderDistance() const { return RenderDistance_; }
	void World::SetRenderDistance(float newRenderDistance) { RenderDistance_ = newRenderDistance; }

	void World::Save(data::FileOutput &output)
	{
		// output.Get() = data::Json {{"version", "0.01"}, {"entities", data::Json::array()}};

		// for(const auto &entity : ECSInstance_->GetComponentPool<ComponentDispatcher>())
		// {
		// 	auto *c = ECSInstance_->GetComponent<ComponentDispatcher>(entity);
		// 	if(c == nullptr) continue;

		// 	data::Json out = data::Json::object();
		// 	c->Dispatch((dstd::USize)CommonMessages::SaveMessage, &out);
		// 	output.Get()["entities"].push_back(data::Json {{"id", entity}, {"components", comps}});
		// }

		// for(const auto &entity : ECSInstance_)
		// {
		// 	data::Json comps = data::Json::array();

		// 	const auto &systems = ECSInstance()->GetSystems(entity);
		// 	for(const auto &system : systems)
		// 	{
		// 		system->SaveFunction(entity, out);

		// 		comps.push_back(out);
		// 	}

		// 	output.Get()["entities"].push_back(data::Json {{"id", entity}, {"components", comps}});
		// }
	}

	void World::Load(const data::FileInput &input)
	{
		// if(input.Get()["version"] != "0.01")
		// {
		// 	DCORE_LOG_ERROR << "Incorrect savefile version: '" << input.Get()["version"] << "', errors may occur.";
		// 	// TODO: Create backup here.
		// }

		// const auto &entities  = input.Get()["entities"];
		// unsigned int entityNo = 0;
		// for(const auto &ej : entities)
		// {
		// 	EntityHandle id = ECSInstance()->CreateEntity();

		// 	if(!ej.contains("uuid"))
		// 	{
		// 		LOG_F(WARNING, "Loading entity without a UUID! (Entity #%u)", entityNo);
		// 	}
		// 	else
		// 	{
		// 		auto uuidJson = ej["uuid"].get<std::string>();
		// 		dstd::UUID uuid;
		// 		dstd::UUID::Parse(uuid, uuidJson);
		// 		ECSInstance()->AddComponent(id, UUIDComponent(std::move(uuid)));
		// 	}

		// 	for(const auto &comp : ej["components"])
		// 	{
		// 		const auto &system = ECSInstance()->GetSystemByName(comp["@type"]);
		// 		ECSInstance()->AddEntityToSystem(system, id);
		// 	}

		// 	entityNo += 1;
		// }
	}
} // namespace dcore::world
