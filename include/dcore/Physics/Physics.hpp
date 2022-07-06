#pragma once
#include <dcore/Core/Type.hpp>
#include <dcore/Core/Span.hpp>
#include <reactphysics3d/reactphysics3d.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace dcore::physics
{
	namespace rp3d = reactphysics3d;
	class PhysicsWorld;
	class CubeCollider;
	class SphereCollider;
	class CapsuleCollider;
	class TerrainCollider;
	class ConvexCollider;

	class Physics
	{
	public:
		static Physics *Instance();
		PhysicsWorld *CreatePhysicsWorld();

		SphereCollider *CreateSphereCollider(float radius);
		CubeCollider *CreateCubeCollider(glm::vec3 extents);
		CapsuleCollider *CreateCapsuleCollider(float radius, float height);
		ConvexCollider *CreateConvexCollider(dstd::Span<float> vertices, dstd::Span<dstd::USize> indices);
		TerrainCollider *CreateTerrainCollider(... /* TODO */);
		void DestroySphereCollider(SphereCollider *collider);
		void DestroyCubeCollider(CubeCollider *collider);
		void DestroyCapsuleCollider(CapsuleCollider *collider);
		void DestroyConvexCollider(ConvexCollider *collider);
		void DestroyTerrainCollider(TerrainCollider *collider);
	private:
		rp3d::PhysicsCommon PhysicsCommon_;
	};

	namespace detail
	{
		inline glm::vec3 Convert(rp3d::Vector3 v) { return { v.x, v.y, v.z }; }
		inline glm::quat Convert(rp3d::Quaternion v) { return { v.w, v.x, v.y, v.z }; }
		inline rp3d::Vector3 Convert(glm::vec3 v) { return { v.x, v.y, v.z }; }
		inline rp3d::Quaternion Convert(glm::quat v) { return { v.x, v.y, v.z, v.w }; }
	}
}
