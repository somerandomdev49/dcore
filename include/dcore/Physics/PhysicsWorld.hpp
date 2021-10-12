#pragma once
#include <dcore/World/World.hpp>
#include <glm/glm.hpp>
#include <vector>

namespace dcore::physics
{
	enum ColliderType
	{
		NoColliderType,
		BoxColliderType,
		CapsuleColliderType,
		PolygonColliderType,
	};

	struct CollisionInfo
	{
		glm::vec3 MTV_;
	};

	class Collider
	{
	public:
		virtual ~Collider();
		ColliderType GetType() const;
		virtual bool DoesOverlap(Collider *other) = 0;

	protected:
		ColliderType Type_;
	};

	// class PhysicsWorld;
	class KinematicBodyComponent
	{
	public:
		Collider *GetCollider() const;

	private:
		friend class PhysicsWorld;
		Collider *Collider_;
	};

	class PhysicsWorld
	{
	public:
		void Initialize(world::World *world);
		void DeInitialize();

	private:
		void CollideAndResolve();
		world::World *World_;
	};
} // namespace dcore::physics
