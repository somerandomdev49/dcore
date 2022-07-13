#include <dcore/Physics/PhysicsWorld.hpp>
#include <dcore/Physics/CollisionBody.hpp>

namespace dcore::physics
{
	void PhysicsWorld::CreateCollisionBody(CollisionBody *body, glm::vec3 position, glm::quat rotation)
	{
		auto *c = World_->createCollisionBody(rp3d::Transform(detail::Convert(position), detail::Convert(rotation)));
		body->Initialize(c);
	}
} // namespace dcore::physics
