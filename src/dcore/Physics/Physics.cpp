#include <dcore/Physics/Physics.hpp>

namespace dcore::physics
{
	static Physics PhysicsInstance_;
	Physics *Physics::Instance() { return &PhysicsInstance_; }
}
