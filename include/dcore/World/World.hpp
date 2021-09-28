#pragma once
#include <dcore/Resource/ResourceManager.hpp>
#include <dcore/Graphics/Graphics.hpp>
#include <entt/entity/registry.hpp>
#include <fwdraw.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace dcore::platform { class Context; }

namespace dcore::world
{
    struct TransformComponent
    {
        entt::entity Parent;
        glm::vec3 Position;
        glm::quat Rotation;
        glm::vec3 Scale;

        glm::mat4 Matrix;

        /** Recalculates the transform matrix. */
        void ReCalculateMatrix();
    };

    struct ModelRenderableComponent
    {
        graphics::StaticMesh Mesh;
    };

    struct DynamicComponent
    {
        void *Data;
        void (*Update)(void*, World*);
    };

    class World;

    class Entity
    {
    public:
        Entity(entt::entity id, World *world);

        template<typename T>
        void AddComponent(const T &c);

        template<typename T>
        T &GetComponent() const;

        /** Returns the entity's id. */
        entt::entity GetId() const;
    private:
        friend class World;
        entt::entity Id_;
        World *World_;
    };
    
    class World
    {
    public:
        template<typename T>
        T &GetComponent(const Entity *entity);
        
        template<typename T>
        void AddComponent(Entity *entity, const T &c);

        Entity CreateEntity();
    private:
        friend class platform::Context;
        void Update();
        void Render(graphics::RendererInterface *render);
        entt::registry Registry_;
    };
}

template<typename T>
T &dcore::world::Entity::GetComponent() const
{ return World_->GetComponent<T>(this); }

template<typename T>
void dcore::world::Entity::AddComponent(const T &c)
{ return World_->AddComponent<T>(this, c); }

template<typename T>
T &dcore::world::World::GetComponent(const Entity *entity)
{ return Registry_.get<T>(entity->Id_); }

template<typename T>
void dcore::world::World::AddComponent(Entity *entity, const T &c)
{ Registry_.emplace<T>(entity->Id_, c); }
