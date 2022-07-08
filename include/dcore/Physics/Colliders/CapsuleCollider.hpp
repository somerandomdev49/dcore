#pragma once
#include <dcore/Physics/Physics.hpp>
#include <dcore/Physics/Collider.hpp>

namespace dcore::physics
{
    struct RCapsuleCollider
    {
        float Height, Radius;
    };

    class CapsuleCollider : public Collider
    {
    public:
        using ResourceType = RCapsuleCollider;
        void Initialize(Physics *physics, void *resource) override;
        void DeInitialize(Physics *physics) override;
    };
}
