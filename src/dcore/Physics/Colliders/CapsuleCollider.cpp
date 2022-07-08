#include <dcore/Physics/Colliders/CapsuleCollider.hpp>

namespace dcore::physics
{
	void CapsuleCollider::Initialize(Physics *physics, void *resource)
	{
		auto *collider = (RCapsuleCollider *)resource;
		
		Shape_ = physics->GetCommon().createCapsuleShape(
			collider->Radius,
			collider->Height
		);
	}

	void CapsuleCollider::DeInitialize(Physics *physics)
	{
		physics->GetCommon().destroyCapsuleShape((rp3d::CapsuleShape*)Shape_);
	}
}
