#pragma once
#include <dcore/Physics/Physics.hpp>
#include <dcore/Physics/Collider.hpp>

namespace dcore::physics
{
    struct RSphereCollider
    {
        float Radius;
    };

    class SphereCollider : public Collider
    {
    public:
        using ResourceType = RSphereCollider;
        void Initialize(Physics *physics, void *resource) override;
        void DeInitialize(Physics *physics) override;
    };
}
