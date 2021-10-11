#include <dcore/World/World.hpp>
#include <dcore/Graphics/Graphics.hpp>

using namespace dcore::world;

void TransformComponent::ReCalculateMatrix()
{
    Matrix = glm::mat4_cast(Rotation);
    Matrix = glm::scale(Matrix, Scale);
    Matrix = glm::translate(Matrix, Position);
}

void World::Initialize() { /* WInfo_.World_ = this; */ }
void World::DeInitialize() { }


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
    auto v = Registry_.view<StaticMeshComponent, TransformComponent>();

    for(auto e : v)
    {
        auto &t = Registry_.get<TransformComponent>(e);
        auto &r = Registry_.get<StaticMeshComponent>(e);
        t.ReCalculateMatrix();
        r.Mesh.SetTransform(t.Matrix); // TODO: Pass this as parameter to RenderStaticMesh
        render->RenderStaticMesh(&r.Mesh);
    }
}

entt::entity Entity::GetId() const { return Id_; }
Entity::Entity(entt::entity e, World *world)
    : Id_(e), World_(world) {}

Entity World::CreateEntity() { return Entity(Registry_.create(), this); }

void World::RegisterUpdate(void (*f)(World*)) { Updates_.push_back(f); }
