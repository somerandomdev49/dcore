#include <dcore/Physics/ColliderComponent.hpp>

namespace dcore::physics
{	
	void ColliderComponent::Start(const dcore::world::EntityHandle &self)
	{
		Collider_->Initialize(Physics::Instance(), Resource_);
	}

	void ColliderComponent::End(const dcore::world::EntityHandle &self)
	{
		Collider_->DeInitialize(Physics::Instance());
	}
}
