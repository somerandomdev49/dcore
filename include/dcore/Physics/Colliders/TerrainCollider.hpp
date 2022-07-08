#pragma once
#include <dcore/Physics/Physics.hpp>
#include <dcore/Physics/Collider.hpp>
#include <vector>

namespace dcore::physics
{
    struct RTerrainCollider
    {
        std::vector<float> Heights;
        glm::uvec2 Size;
        float MinHeight, MaxHeight;
        float Scale;
    };

    class TerrainCollider : public Collider
    {
    public:
        using ResourceType = RTerrainCollider;
        void Initialize(Physics *physics, void *resource) override;
        void DeInitialize(Physics *physics) override;
    };
}
