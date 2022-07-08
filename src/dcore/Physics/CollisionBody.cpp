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
		Body_->setTransform({ detail::Convert(newPosition), detail::Convert(newRotation) });
	}
}
