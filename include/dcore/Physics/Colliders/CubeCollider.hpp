#pragma once
#include <dcore/Physics/Physics.hpp>
#include <dcore/Physics/Collider.hpp>

namespace dcore::physics
{
    struct RCubeCollider
    {
        glm::vec3 Extents;
    };

    class CubeCollider : public Collider
    {
    public:
        using ResourceType = RCubeCollider;
        void Initialize(Physics *physics, void *resource) override;
        void DeInitialize(Physics *physics) override;
    };
}
