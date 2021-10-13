#pragma once
#include <dcore/World/World.hpp>
#include <glm/glm.hpp>
#include <vector>
#define DCORE_GRAVITY_Y -9.8f
namespace dcore::physics
{
	enum ColliderType
	{
		// No collision at all.
		NoColliderType,

		// Collision with pawns.
		PawnColliderType,

		// Collision with terrrain.
		TerrainColliderType,

		// Collision with the world. (houses, trees)
		WallColliderType,

		ColliderTypeCount
	};

	struct CollisionInfo
	{
		glm::vec3 MTV_;
	};

	class AABB
	{
		glm::vec3 Min, Max;
	};

	class Collider
	{
	public:
		Collider(AABB &&broadCollider);
		virtual ~Collider();

		ColliderType GetType() const;
		const AABB &GetBroadCollider() const;

		bool DoesOverlap(Collider *other);

	protected:
		using CollisionFunction = bool (*)(Collider *other);
		CollisionFunction NarrowPhaseColliders[ColliderTypeCount];

		AABB BroadCollider_;
		ColliderType Type_;
	};

	// Kinematic body (an Actor) physics.
	class ActorPhysicsComponent
	{
	public:
		Collider *GetCollider() const;

	private:
		friend class PhysicsWorld;
		glm::vec3 Velocity_;
		glm::vec3 Acceleration_;
		Collider *Collider_;
	};

	class WorldColliderComponent
	{
	public:
	};

	class PhysicsWorld
	{
	public:
		void Initialize(world::World *world);
		void DeInitialize();

	private:
		void DoBroadPhase();
		void DoNarrowPhase();
		void CollideAndResolve();
		world::World *World_;
	};
} // namespace dcore::physics
