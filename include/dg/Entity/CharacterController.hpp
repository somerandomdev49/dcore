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
		bool IsFalling() const { return IsFalling_; }
	private:
		dcore::world::TransformComponent *TransformComponent_ = nullptr;
		float Yaw_ = 0;
		bool IsMoving_ = false, IsFalling_ = false;
		float Speed_ = 20.0f;
		float Gravity_ = 10.0f;
		glm::vec3 Velocity_ = glm::vec3(0, 0, 0);
	};
} // namespace dg::entity
