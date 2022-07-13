#pragma once
#include <dcore/Physics/Physics.hpp>
#include <dcore/Physics/Collider.hpp>
#include <vector>

namespace dcore::physics
{
    // TODO: Docs in RVertexCollider
    /**
     * @brief Convex Collider resource
     * @see ConvexCollider
     * @link https://www.reactphysics3d.com/usermanual.html#x1-5100011.1.4
     */
    struct RConvexCollider
    {
        /** Array of polygon faces */
        std::vector<rp3d::PolygonVertexArray::PolygonFace> Faces;
        /** Vertices */
        rp3d::PolygonVertexArray *VetexArray;
        /** Mesh */
        rp3d::PolyhedronMesh *Polyhedron;
    };

    /**
     * @brief Convex Collider. Represents @b convex polygon shapes
     * @see RConvexCollider
     */
    class ConvexCollider : public Collider
    {
    public:
        using ResourceType = RConvexCollider;
        void Initialize(Physics *physics, void *resource) override;
        void DeInitialize(Physics *physics) override;
    };
}
