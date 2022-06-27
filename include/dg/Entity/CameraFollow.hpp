#pragma once
#include <dcore/World/Entity.hpp>
#include <dcore/World/World.hpp>
#include <dg/Entity/CharacterController.hpp>

namespace dg::entity
{
	class CameraFollowComponent : public dcore::world::ComponentBase<CameraFollowComponent>
	{
	public:
		CameraFollowComponent(dcore::graphics::Camera *camera);

		void Start(dcore::world::EntityHandle self);
		void Update(dcore::world::EntityHandle self);

		static constexpr float DefaultOffset_ = 40.0f;

	private:
		void UpdateCameraRotation_(float xAngle, float yAngle);

		dcore::world::TransformComponent *Transform_         = nullptr;
		dcore::graphics::Camera *Camera_                     = nullptr;
		dg::entity::CharacterControllerComponent *Character_ = nullptr;

		float Offset_ = DefaultOffset_, Timer_ = 0;
		int CameraMode_         = 0;
		glm::vec2 LastMousePos_ = {0, 0}, Total_ = {0, 0};
		glm::mat4 Rotation_ = glm::mat4(1);
	};
} // namespace dg::entity
