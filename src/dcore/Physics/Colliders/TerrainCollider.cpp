#include <dcore/Physics/Colliders/TerrainCollider.hpp>

namespace dcore::physics
{
	void TerrainCollider::Initialize(Physics *physics, void *resource)
	{
		auto *collider = (RTerrainCollider *)resource;

		Shape_ = physics->GetCommon().createHeightFieldShape(
			collider->Size.x,
			collider->Size.y,
			collider->MinHeight,
			collider->MaxHeight,
			collider->Heights.data(),
			rp3d::HeightFieldShape::HeightDataType::HEIGHT_FLOAT_TYPE
		);
	}

	void TerrainCollider::DeInitialize(Physics *physics)
	{
		physics->GetCommon().destroyHeightFieldShape((rp3d::HeightFieldShape*)Shape_);
	}
}
