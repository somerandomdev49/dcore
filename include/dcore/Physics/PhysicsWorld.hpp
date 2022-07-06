#pragma once
#include <dcore/Physics/Physics.hpp>

namespace dcore::physics
{
	class CollisionBody;
	class RigidBody;

	class PhysicsWorld
	{
	public:
		Physics *GetPhysics() const { return Physics_; }

		CollisionBody *CreateCollisionBody(glm::vec3 position = { 0, 0, 0 }, glm::quat rotation = { 1, 0, 0, 0 });
		RigidBody *CreateRigidBody(glm::vec3 position = { 0, 0, 0 }, glm::quat rotation = { 1, 0, 0, 0 });

		void DestroyCollisionBody(CollisionBody *body);
		void DestroyRigidBody(RigidBody *body) {}
	private:
		Physics *Physics_;
		rp3d::PhysicsWorld *World_;
	};
}
