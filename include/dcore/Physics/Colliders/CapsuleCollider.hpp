#pragma once
#include <dcore/Physics/Physics.hpp>
#include <dcore/Physics/Collider.hpp>

namespace dcore::physics
{
    class CapsuleCollider : public Collider
    {
    private:
        friend class Physics;
        rp3d::CapsuleShape *Shape_;
    };
}
