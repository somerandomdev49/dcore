#pragma once
#include <dcore/World/Entity.hpp>
#include <dcore/World/World.hpp>
#include <glm/glm.hpp>

namespace dg::entity
{
	class CharacterControllerComponent : public dcore::world::ComponentBase<CharacterControllerComponent>
	{
	public:
		void Start(const dcore::world::EntityHandle &self);
		void Update(const dcore::world::EntityHandle &self);

		float GetYaw() const { return Yaw_; }
		void SetYaw(float newYaw) { Yaw_ = newYaw; }
		bool IsMoving() const { return IsMoving_; }
		void SetIsMoving(bool isMoving, glm::vec2 movement)
		{
			IsMoving_ = isMoving;
			Movement_ = movement;
		}

	private:
		dcore::world::TransformComponent *TransformComponent_ = nullptr;
		float Yaw_                                            = 0;
		bool IsMoving_ = false, IsFalling_ = false;
		glm::vec2 Movement_;
		float Speed_        = 20.0f;
		float Gravity_      = 10.0f;
		glm::vec3 Velocity_ = glm::vec3(0, 0, 0);
	};
} // namespace dg::entity
