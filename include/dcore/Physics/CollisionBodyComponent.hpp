#pragma once
#include <dcore/Physics/CollisionBody.hpp>
#include <dcore/World/World.hpp>

namespace dcore::physics
{
	/**
	 * @brief Entity component that holds a @ref dcore::physics::CollisionBody
	 */
	class CollisionBodyComponent : public world::ComponentBase<CollisionBodyComponent>
	{
		CollisionBody &GetCollisionBody() { return CollisionBody_; }
		const CollisionBody &GetCollisionBody() const { return CollisionBody_; }
	private:
		CollisionBody CollisionBody_;
	};
} // namespace dcore::physics
