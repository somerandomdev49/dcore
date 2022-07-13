#pragma once
#include <dcore/Physics/Physics.hpp>

namespace dcore::physics
{
	/**
	 * @brief Abstract base class of all colliders.
	 * @see reactphysics3d::CollisionShape
	 */
	class Collider
	{
	public:
		/**
		 * @brief Initializes the collider given the resource.
		 * 
		 * @param physics Physics instance (usually `Physics::Instance()`)
		 * @param resource Resource of the collider type
		 */
		virtual void Initialize(Physics *physics, void *resource) = 0;

		/**
		 * @brief DeInitializes the collider
		 * 
		 * @param physics Physics instance, identical to one provided in @ref Initialize
		 */
		virtual void DeInitialize(Physics *physics) = 0;

		rp3d::CollisionShape *GetShape() const { return Shape_; }

	protected:
		rp3d::CollisionShape *Shape_;
	};
} // namespace dcore::physics
