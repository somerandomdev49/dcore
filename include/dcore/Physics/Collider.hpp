#pragma once
#include <dcore/Physics/Physics.hpp>

namespace dcore::physics
{
	class Collider
	{
	public:
		virtual void Initialize(Physics *physics, void *resource);
		virtual void DeInitialize(Physics *physics);

		rp3d::CollisionShape *GetShape() const { return Shape_; }

	protected:
		rp3d::CollisionShape *Shape_;
	};
} // namespace dcore::physics
