#include <dcore/Physics/PhysicsWorld.hpp>

namespace dcore::physics
{
	class NoCollider : public Collider
	{
		bool DoesOverlap(Collider *other); // returns false
	};

	class BoxCollider : public Collider
	{
		bool DoesOverlap(Collider *other); // returns false
	};

	class CapsuleCollider : public Collider
	{
		bool DoesOverlap(Collider *other); // returns false
	};

	class NoCollider : public Collider
	{
		bool DoesOverlap(Collider *other); // returns false
	};
} // namespace dcore::physics
