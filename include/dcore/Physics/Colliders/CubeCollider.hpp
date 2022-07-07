#pragma once
#include <dcore/Physics/Physics.hpp>
#include <dcore/Physics/Collider.hpp>

namespace dcore::physics
{
    class CubeCollider : public Collider
    {
    private:
        friend class Physics;
        rp3d::BoxShape *Shape_;
    };
}
