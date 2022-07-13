#pragma once
#include <dcore/Physics/Physics.hpp>
#include <dcore/Physics/Collider.hpp>

namespace dcore::physics
{
    /**
     * @brief Sphere Collider resource
     * @see ShpereCollider
     */
    struct RSphereCollider
    {
        /** Sphere radius */
        float Radius;
    };

    /**
     * @brief Sphere Collider
     * @see RShpereCollider
     */
    class SphereCollider : public Collider
    {
    public:
        using ResourceType = RSphereCollider;
        void Initialize(Physics *physics, void *resource) override;
        void DeInitialize(Physics *physics) override;
    };
}
