#include <dcore/World/World.hpp>
#include <dcore/Graphics/Graphics.hpp>
#include <dcore/Graphics/GUI/GuiGraphics.hpp>
#include <dcore/Graphics/GUI/Font.hpp>

using namespace dcore::world;

void TransformComponent::ReCalculateMatrix()
{
	Matrix = glm::mat4_cast(Rotation);
	Matrix = glm::scale(Matrix, Scale);
	Matrix = glm::translate(Matrix, Position);
}

dcore::resource::Resource<dcore::graphics::gui::Font> font__tmp;
void                                                  World::Initialize()
{
	// TODO: This should not be constant!
	Terrain_.Initialize(resource::ResourceManager::Instance()->Get<terrain::Heightmap>("DCore.Heightmap.World1"));
	Terrain_.ActivateAllChunks();
	font__tmp = resource::ResourceManager::Instance()->Get<graphics::gui::Font>("DCore.Font.Debug");
}

void World::DeInitialize() {}

void World::Update()
{
	auto v = Registry_.view<DynamicComponent>();

	for(const auto e : v)
	{
		const auto &dc = Registry_.get<DynamicComponent>(e);
		dc.Update(dc.Data, this);
	}

	for(const auto p : Updates_) p(this);
}

void World::Render(graphics::RendererInterface *render)
{
	Terrain_.ReactivateChunks(render->GetCamera()->GetPosition(), RenderDistance_);
	auto v = Registry_.view<StaticMeshComponent, TransformComponent>();

	for(auto e : v)
	{
		auto &t = Registry_.get<TransformComponent>(e);
		auto &r = Registry_.get<StaticMeshComponent>(e);
		t.ReCalculateMatrix();
		r.Mesh.SetTransform(t.Matrix);
		render->RenderStaticMesh(&r.Mesh);
	}

	auto &chunks = Terrain_.GetChunks();
	for(auto ci : Terrain_.GetActiveChunks()) render->RenderChunk(&chunks[ci]);

	char        posString[32];
	const auto &camPos = render->GetCamera()->GetPosition();
	snprintf(posString, 32, "%.1f, %.1f, %.1f", camPos.x, camPos.y, camPos.z);
	graphics::gui::GuiGraphics::Instance()->RenderText(font__tmp.Get(), posString, glm::vec2(4, 20), 24);
	// graphics::gui::GuiGraphics::Instance()->RenderQuad(graphics::gui::Quad {glm::vec2(0.1f, 0.1f), glm::vec2(0.5f,
	// 0.5f), 0.0f,
	// font__tmp.Get()->GetAtlasTexture(),
	// glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)});
}

entt::entity Entity::GetId() const { return Id_; }
Entity::Entity(entt::entity e, World *world) : Id_(e), World_(world) {}

Entity World::CreateEntity() { return Entity(Registry_.create(), this); }

void World::RegisterUpdate(void (*f)(World *)) { Updates_.push_back(f); }

float World::GetRenderDistance() const { return RenderDistance_; }
void  World::SetRenderDistance(float newRenderDistance) { RenderDistance_ = newRenderDistance; }
