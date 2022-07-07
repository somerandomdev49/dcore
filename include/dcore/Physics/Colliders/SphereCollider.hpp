#pragma once
#include <dcore/Physics/Physics.hpp>
#include <dcore/Physics/Collider.hpp>

namespace dcore::physics
{
    class SphereCollider : public Collider
    {
    private:
        friend class Physics;
        rp3d::SphereShape *Shape_;
    };
}
