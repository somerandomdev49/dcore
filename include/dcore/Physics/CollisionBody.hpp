#pragma once
#include <dcore/Physics/Physics.hpp>
#include <dcore/Physics/PhysicsWorld.hpp>
#include <dcore/Physics/Collider.hpp>

namespace dcore::physics
{
	class CollisionBody
	{
	public:
		void SetPosition(glm::vec3 newPosition) { UpdatePosition_(newPosition); }
		void SetRotation(glm::quat newRotation) { UpdateRotation_(newRotation); }
		glm::vec3 GetPosition() const { return detail::Convert(Body_->getTransform().getPosition()); }
		glm::quat GetRotation() const { return detail::Convert(Body_->getTransform().getOrientation()); }

		void AddCollider(Collider *shape);

		PhysicsWorld *GetWorld() const { return World_; }
	private:
		friend class PhysicsWorld;
		CollisionBody(rp3d::CollisionBody *body) : Body_(body) { }

		void UpdateTransform_(glm::vec3 newPosition, glm::quat newRotation);
		void UpdatePosition_(glm::vec3 newPosition) { UpdateTransform_(newPosition, GetRotation()); }
		void UpdateRotation_(glm::quat newRotation) { UpdateTransform_(GetPosition(), newRotation); }

		PhysicsWorld *World_;
		rp3d::CollisionBody *Body_;
	};
}
