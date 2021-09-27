#include <dcore/World/World.hpp>
#include <dcore/Graphics/Graphics.hpp>

using namespace dcore::world;

void World::Update()
{
    auto v = Registry_.view<DynamicComponent>();

    for(const auto e : v)
    {
        const auto &dc = Registry_.get<DynamicComponent>(e);
        dc.Update(dc.Data, this);
    }
}

void World::Render(graphics::RendererInterface *render)
{
    auto v = Registry_.view<ModelRenderableComponent>();

    for(const auto e : v)
    {
        const auto &r = Registry_.get<ModelRenderableComponent>(e);
        render->RenderStaticMesh(&r.Mesh);
    }
}

entt::entity Entity::GetId() const { return Id_; }
Entity::Entity(entt::entity e, World *world)
    : Id_(e), World_(world) {}

Entity World::CreateEntity() { return Entity(Registry_.create(), this); }
