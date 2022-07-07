#pragma once
#include <dcore/Physics/Physics.hpp>
#include <dcore/Physics/Collider.hpp>
#include <vector>

namespace dcore::physics
{
    struct RTerrainCollider
    {
        std::vector<float> Heights;
    };

    class TerrainCollider : public Collider
    {
    private:
        friend class Physics;
        RTerrainCollider *ColliderData_;
        rp3d::HeightFieldShape *Shape_;
    };
}
