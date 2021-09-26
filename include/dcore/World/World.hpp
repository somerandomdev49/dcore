#pragma once
#include <entt/entity/registry.hpp>

namespace dcore::platform { class Context; }

namespace dcore::world
{
    struct TransformComponent
    {
        entt::entity Parent;
        glm::vec3 Position;
        glm::quat Rotation;
        glm::vec3 Scale;
    }

    class World;

    class Entity
    {
    public:
        template<typename T>
        T &GetComponent();
    private:
        World *World_;
        entt::entity Id_;
    };
    
    class World
    {
    public:
        /** Returns a component of a specified type. */
        template<typename T>
        T &GetComponent(Entity *entity);

        /** Returns the qntity's id. */
        entt::id_type GetId();
    private:
        friend class platform::Context;
        void Update();
        void Render();
        entt::registry Registry_;
    }
}

template<typename T>
T &dcore::world::Entity::GetComponent()
{ return World_->GetComponent<T>(this); }


template<typename T>
T &dcore::world::World::GetComponent(Entity *entity)
{ return Registry_.get<T>(entity->Id_); }
