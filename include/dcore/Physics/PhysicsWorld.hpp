#pragma once
#include <dcore/Physics/Physics.hpp>

namespace dcore::physics
{
	class CollisionBody;
	class RigidBody;

	/**
	 * @brief Contains all of the bodies and their interactions.
	 * @see reactphysics3d::PhysicsWorld
	 */
	class PhysicsWorld
	{
	public:
		Physics *GetPhysics() const { return Physics_; }
		rp3d::PhysicsWorld *GetWorld() const { return World_; }
	private:
		Physics *Physics_;
		rp3d::PhysicsWorld *World_;
	};
} // namespace dcore::physics
