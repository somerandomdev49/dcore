#pragma once
#include <dcore/Physics/Physics.hpp>
#include <dcore/Physics/Collider.hpp>
#include <vector>

namespace dcore::physics
{
    struct RConvexCollider
    {
        std::vector<rp3d::PolygonVertexArray::PolygonFace> Faces;
        rp3d::PolygonVertexArray *VetexArray;
        rp3d::PolyhedronMesh *Polyhedron;
    };

    class ConvexCollider : public Collider
    {
    private:
        friend class Physics;
        RConvexCollider *ColliderData_;
        rp3d::ConvexShape *Shape_;
    };
}
