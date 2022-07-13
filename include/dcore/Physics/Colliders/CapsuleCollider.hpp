#pragma once
#include <dcore/Physics/Physics.hpp>
#include <dcore/Physics/Collider.hpp>

namespace dcore::physics
{
    /**
     * @brief Capsule Collider resource.
     * @see CapsuleCollider
     */
    struct RCapsuleCollider
    {
        /** Height of the capsule. */
        float Height;

        /** Radius of the capsule */
        float Radius;
    };

    /**
     * @brief Capsule Collider.
     * @see RCapsuleCollider
     */
    class CapsuleCollider : public Collider
    {
    public:
        using ResourceType = RCapsuleCollider;
        void Initialize(Physics *physics, void *resource) override;
        void DeInitialize(Physics *physics) override;
    };
}
