#include <dcore/Physics/CollisionBody.hpp>

namespace dcore::physics
{
	void CollisionBody::AddCollider(Collider *collider)
	{
		// TODO: Transform
		Body_->addCollider(collider->GetShape(), rp3d::Transform::identity());
	}

	void CollisionBody::UpdateTransform_(glm::vec3 newPosition, glm::quat newRotation)
	{
		Body_->setTransform({detail::Convert(newPosition), detail::Convert(newRotation)});
	}

	void CollisionBody::SetPosition(glm::vec3 newPosition) { UpdateTransform_(newPosition, GetRotation()); }
	void CollisionBody::SetRotation(glm::quat newRotation) { UpdateTransform_(GetPosition(), newRotation); }
	glm::vec3 CollisionBody::GetPosition() const { return detail::Convert(Body_->getTransform().getPosition()); }
	glm::quat CollisionBody::GetRotation() const { return detail::Convert(Body_->getTransform().getOrientation()); }
	PhysicsWorld *CollisionBody::GetWorld() const { return World_; }

	void CollisionBody::Initialize(PhysicsWorld *world, glm::vec3 position, glm::quat rotation)
	{
		World_ = world;
		Body_  = world->GetWorld()->createCollisionBody(
		     rp3d::Transform(detail::Convert(position), detail::Convert(rotation)));
	}

	void CollisionBody::DeInitialize()
	{
		World_->GetWorld()->destroyCollisionBody(Body_);
		Body_ = nullptr;
	}
} // namespace dcore::physics
