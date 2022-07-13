#pragma once
#include <dcore/Physics/Physics.hpp>
#include <dcore/Physics/PhysicsWorld.hpp>
#include <dcore/Physics/Collider.hpp>

namespace dcore::physics
{
	/**
	 * @brief Physics body that collides with others but isn't affected by physics itself.
	 * @see reactphysics3d::CollisionBody
	 */
	class CollisionBody
	{
	public:
		void SetPosition(glm::vec3 newPosition);
		void SetRotation(glm::quat newRotation);
		glm::vec3 GetPosition() const;
		glm::quat GetRotation() const;
		PhysicsWorld *GetWorld() const;

		/**
		 * @brief Adds a collider to the body.
		 * @todo Add a transform parameter
		 * @param collider The collider to be added.
		 */
		void AddCollider(Collider *collider);

		/**
		 * @brief Creates the collision body with given transform.
		 * 
		 * @param world The world that the body is in.
		 * @param position Initial position of the body.
		 * @param rotation Initial rotation of the body.
		 */
		void Initialize(PhysicsWorld *world, glm::vec3 position = {0, 0, 0}, glm::quat rotation = {1, 0, 0, 0});

		/**
		 * @brief Destroys the internal collision body and sets it to null
		 */
		void DeInitialize();

	private:
		void UpdateTransform_(glm::vec3 newPosition, glm::quat newRotation);

		PhysicsWorld *World_;
		rp3d::CollisionBody *Body_;
	};
} // namespace dcore::physics
