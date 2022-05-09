#include <algorithm>
#include <dcore/World/World.hpp>
#include <dcore/Graphics/GUI/GuiGraphics.hpp>
#include <dcore/Graphics/GUI/GuiManager.hpp>
#include <dcore/Util/JsonConverters.hpp>
#include <dcore/Graphics/GUI/Font.hpp>
#include <dcore/Graphics/Graphics.hpp>
#include <dcore/Platform/Platform.hpp>
#include <dcore/Core/Preferences.hpp>
#include <iostream>
#include <iterator>
#include <memory>

namespace dcore::world
{
	WorldMessageHandlerProvider *WorldMessageHandlerProviderInstance = nullptr;
	WorldMessageHandlerProvider *WorldMessageHandlerProvider::Instance()
	{
		if(WorldMessageHandlerProviderInstance == nullptr)
			WorldMessageHandlerProviderInstance = new WorldMessageHandlerProvider();
		return WorldMessageHandlerProviderInstance;
	}

	void WorldMessageHandlerProvider::AddHandler(World::MessageHandlerFunc func) { Handlers_.push_back(func); }

	DCORE_COMPONENT_REGISTER(TransformComponent);
	DCORE_COMPONENT_REGISTER(StaticMeshComponent);
	DCORE_COMPONENT_REGISTER(ModelComponent);
	DCORE_COMPONENT_REGISTER(DynamicComponent);
	DCORE_COMPONENT_REGISTER(TerrainComponent);

	void StaticMeshComponent::Render(EntityHandle self, graphics::RendererInterface *render) const
	{
		// LOG_F(INFO, "ModelComponent | Render");
		// auto *transform =
		// platform::Context::Instance()->GetWorld()->GetECS()->GetComponent<TransformComponent>(self);
		// render->RenderStaticMesh(&Mesh, transform->GetMatrix());
	}

	// void StaticMeshComponent::Save(const EntityHandle &self, data::Json &output) const
	// {
	// 	(void)self;
	// 	std::string name = Mesh.GetTexture().GetName();
	// 	std::cout << "Saving StaticMeshComponent, GetTexture().GetName() = " << name << std::endl;
	// 	auto meshInfo = data::Json {
	// 	    {"mesh", Mesh.GetMesh().GetName()},
	// 	    {"texture", Mesh.GetTexture().GetName()},
	// 	};

	// 	output = data::Json {{"mesh", meshInfo}};
	// }

	void TransformComponent::Save(const EntityHandle &entity, data::Json &output) const
	{
		(void)entity;
		output = data::Json::object({
		    {"position", util::JsonConverters::Glm(this->Position_)},
		    {"rotation", util::JsonConverters::Glm(this->Rotation_)},
		    {"scale", util::JsonConverters::Glm(this->Scale_)},
		});
	}

	void ModelComponent::Render(EntityHandle self, graphics::RendererInterface *render) const
	{
		// LOG_F(INFO, "ModelComponent | Render");
		auto *transform = platform::Context::Instance()->GetWorld()->GetECS()->GetComponent<TransformComponent>(self);
		render->RenderModel(Model.Get(), transform->GetMatrix());
	}
	// void ModelComponent::Save(const EntityHandle &self, data::Json &output) const
	// {
	// 	(void)self;
	// 	output = data::Json::object({{"model", Model.GetName()}});
	// }

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
		LOG_F(WARNING, "TerrainComponent::TerrainComponent()");
		Terrain_.Initialize(heightmap);
		Terrain_.ActivateAllChunks();
		// platform::Context::Instance()->GetWorld()->SetTerrain(&Terrain_);
	}

	void TerrainComponent::Update(const EntityHandle &self)
	{
		Terrain_.ReactivateChunks(platform::Context::Instance()->GetRendererInterface()->GetCamera()->GetPosition(),
		                          platform::Context::Instance()->GetWorld()->GetRenderDistance());
	}

	void World::Initialize()
	{
		RenderDistance_ = Preferences::Instance()->GetGraphicsSettings().RenderDistance;
		ECSInstance_    = new ECS();
		ECSInstance_->Initialize();

		ECSInstance_->SetMessageHandler({this, [](void *self, EntityHandle handle, ECS::Message message)
		                                 {
			                                 ((World *)self)->MessageHandler_(handle, message);
		                                 }});

		std::copy(WorldMessageHandlerProvider::Instance()->Handlers_.begin(),
		          WorldMessageHandlerProvider::Instance()->Handlers_.end(), std::back_inserter(Handlers_));

		Terrain_ = nullptr;

		LOG_F(WARNING, "World::Initialize??");
	}

	void World::DeInitialize()
	{
		if(Terrain_ != nullptr)
		{
			Terrain_->DeInitialize();
			delete Terrain_;
		}

		ECSInstance_->DeInitialize();
		delete ECSInstance_;
	}

	void World::MessageHandler_(EntityHandle handle, ECS::Message message)
	{
		// LOG_F(INFO, "Received message!");
		for(auto handler : Handlers_) handler(this, handle, message);
	}

	terrain::Terrain &TerrainComponent::GetTerrain() { return Terrain_; }

	terrain::Terrain *World::GetTerrain() const { return Terrain_; }

	void World::DispatchMessage_(CommonMessages message, void *data)
	{
		ECSInstance_->Broadcast({(dstd::USize)message, data});
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
		for(auto &debugLayer : DebugLayers_) debugLayer->OnStart();
	}

	void World::End()
	{
		DispatchMessage_(CommonMessages::EndMessage);
		for(auto &debugLayer : DebugLayers_) debugLayer->OnEnd();
	}

	void World::Render(graphics::RendererInterface *render)
	{
		for(auto &debugLayer : DebugLayers_) debugLayer->OnRender(render);

		ECSInstance_->View<ModelComponent, TransformComponent>().Each(
		    [render](EntityHandle handle, ModelComponent &model, TransformComponent &transform)
		    { render->RenderModel(model.Model.Get(), transform.GetMatrix()); });

		ECSInstance_->View<StaticMeshComponent, TransformComponent>().Each(
		    [render](EntityHandle handle, StaticMeshComponent &mesh, TransformComponent &transform)
		    { render->RenderStaticMesh(&mesh.Mesh, transform.GetMatrix()); });

		Terrain_->ReactivateChunks(render->GetCamera()->GetPosition(), RenderDistance_);
		if(Terrain_ != nullptr)
		{
			const auto &chunks = Terrain_->GetChunks();
			for(auto chunkIndex : Terrain_->GetActiveChunks()) render->RenderChunk(&chunks[chunkIndex]);
		}

		platform::Context::Instance()->GetRendererInterface()->GetRenderer()->DisableDepthCheck();
		graphics::gui::GuiManager::Instance()->Render(graphics::gui::GuiGraphics::Instance());
		platform::Context::Instance()->GetRendererInterface()->GetRenderer()->EnableDepthCheck();
	}

	EntityHandle Entity::GetId() const { return Id_; }
	Entity::Entity(EntityHandle handle, World *world) : Id_(handle), World_(world) {}

	Entity World::CreateEntity() { return {ECSInstance_->Create(), this}; }

	void World::RegisterUpdate(void (*func)(World *)) { Updates_.push_back(func); }

	float World::GetRenderDistance() const { return RenderDistance_; }
	void World::SetRenderDistance(float newRenderDistance) { RenderDistance_ = newRenderDistance; }

	void World::AddDebugLayer(DebugLayer *newDebugLayer)
	{
		DebugLayers_.push_back(std::unique_ptr<DebugLayer>(newDebugLayer));
	}

	void World::CreateTerrain(const resource::Resource<terrain::Heightmap> &heightmap)
	{
		if(Terrain_ != nullptr)
		{
			Terrain_->DeInitialize();
			delete Terrain_;
		}

		Terrain_ = new terrain::Terrain;
		Terrain_->Initialize(heightmap);
	}

	DebugLayer::~DebugLayer() = default;
	void DebugLayer::OnStart() {}
	void DebugLayer::OnRender(graphics::RendererInterface *renderer) {}
	void DebugLayer::OnUpdate() {}
	void DebugLayer::OnEnd() {}
} // namespace dcore::world
