#pragma once
#include <dcore/Physics/Physics.hpp>
#include <dcore/Physics/Collider.hpp>
#include <vector>

namespace dcore::physics
{
    /**
     * @brief TerrainCollider resource.
     * @see TerrainCollider
     */
    struct RTerrainCollider
    {
        /** 2D height data */
        std::vector<float> Heights;

        /** Heightmap size */
        glm::uvec2 Size;

        /** Minimum height in @ref Heights */
        float MinHeight;
        
        /** Maximum height in @ref Heights */
        float MaxHeight;

        /** XZ scale */
        float Scale;
    };

    /**
     * @brief Heightfield collider.
     * @see RTerrainCollider
     */
    class TerrainCollider : public Collider
    {
    public:
        using ResourceType = RTerrainCollider;
        void Initialize(Physics *physics, void *resource) override;
        void DeInitialize(Physics *physics) override;
    };
}
