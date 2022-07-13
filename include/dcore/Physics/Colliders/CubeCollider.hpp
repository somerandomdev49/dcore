#pragma once
#include <dcore/Physics/Physics.hpp>
#include <dcore/Physics/Collider.hpp>

namespace dcore::physics
{
    /**
     * @brief Cube Collider resource
     * @see RCubeCollider
     */
    struct RCubeCollider
    {
        /** Cube size */
        glm::vec3 Extents;
    };

    /**
     * @brief Cube Collider
     * @see RCubeCollider
     */
    class CubeCollider : public Collider
    {
    public:
        using ResourceType = RCubeCollider;
        void Initialize(Physics *physics, void *resource) override;
        void DeInitialize(Physics *physics) override;
    };
}
