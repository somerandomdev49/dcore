#include "dcore/Platform/Window.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/trigonometric.hpp"
#include <dg/Entity/CameraFollow.hpp>

#include <dcore/Core/FrameLog.hpp>
#include <dcore/Platform/Platform.hpp>

DCORE_COMPONENT_REGISTER(dg::entity::CameraFollowComponent);
#define MAX_OFFSET   40.0f
#define MIN_OFFSET   5.0f
#define SCROLL_SPEED 1.5f // TODO: Add to preferences.

namespace
{
	// glm::quat FromEulerZYX(float pitch, float yaw, float roll)
	// {
	// 	// https://gamedev.stackexchange.com/questions/13436/glm-euler-angles-to-quaternion
	// 	float sx = sin(pitch / 2), sy = sin(yaw / 2), sz = sin(roll / 2);
	// 	float cx = cos(pitch / 2), cy = cos(yaw / 2), cz = cos(roll / 2);


	// 	return {
	// 		cx * cy * cz - sx * sy * sz,
	// 		sx * cy * cz + cx * sy * sz,
	// 		cx * sy * cz - sx * cy * sz,
	// 		cx * cy * sz + sx * sy * cz,
	// 	};
	// }

	// glm::quat FromEulerXYZ(float pitch, float yaw, float roll)
	// {
	// 	// https://gamedev.stackexchange.com/questions/13436/glm-euler-angles-to-quaternion
	// 	float sx = sin(pitch / 2), sy = sin(yaw / 2), sz = sin(roll / 2);
	// 	float cx = cos(pitch / 2), cy = cos(yaw / 2), cz = cos(roll / 2);
	// 	return {
	// 		cx*cy*cz + sx*sy*sz,
	// 		sx*cy*cz - cx*sy*sz,
	// 		cx*sy*cz + sx*cy*sz,
	// 		cx*cy*sz - sx*sy*cz,
	// 	};
	// }
}

namespace dg::entity
{
	CameraFollowComponent::CameraFollowComponent(dcore::graphics::Camera *camera) : Camera_(camera) {}

	void CameraFollowComponent::Start(dcore::world::EntityHandle self)
	{
		TransformComponent_ = dcore::world::Entity(self, dcore::platform::Context::Instance()->GetWorld())
		                          .GetComponent<dcore::world::TransformComponent>();

		if(TransformComponent_ == nullptr)
			LOG_F(ERROR, "CameraFollowComponent's entity has to have a TransformComponent!");
		
		// Camera_->SetPosition(TransformComponent_->GetPosition() + glm::vec3(0, 0, Offset_));
	}

	void CameraFollowComponent::Update(dcore::world::EntityHandle self)
	{
		auto *frame = dcore::platform::Context::Instance()->GetFrame();
		if(dcore::event::InputManager::Instance()->IsMousePressed(0))
		{
			frame->SetCursorState(dcore::platform::Frame::CursorState_Grab);
			auto mousePos = dcore::event::InputManager::Instance()->GetMousePosition();

			// constexpr float SENS = .5f;
			auto frameSize = dcore::platform::Context::Instance()->GetFrame()->GetSize();
			glm::vec4 position(Camera_->GetPosition(), 1);
			glm::vec4 pivot(TransformComponent_->GetPosition(), 1);

			float deltaAngleX = 2 * glm::pi<float>() / frameSize.x;
			float deltaAngleY = glm::pi<float>() / frameSize.y;
			float xAngle = (Last_.x - mousePos.x) * deltaAngleX;
			float yAngle = (Last_.y - mousePos.y) * deltaAngleY;

#define EPSILON 0.99f
			float cosAngle = dot(Camera_->GetRotation() * glm::vec3(0, 0, 1), glm::vec3(0, 1, 0));
			if(cosAngle * glm::sign(yAngle) > EPSILON) yAngle = 0;

			Total_.x += xAngle;
			Total_.y -= yAngle;

			Rotation_ = glm::rotate(glm::mat4(1), Total_.x, glm::vec3(0, 1, 0));
			Rotation_ = glm::rotate(Rotation_, Total_.y, glm::vec3(1, 0, 0));
			Last_ = mousePos;
		}
		else
		{
			Last_ = dcore::event::InputManager::Instance()->GetMousePosition();
			frame->SetCursorState(dcore::platform::Frame::CursorState_Normal);
			// Total_ = {0,0};
		}

		Offset_ = glm::clamp(Offset_ - dcore::event::InputManager::Instance()->GetScrollY() * SCROLL_SPEED, MIN_OFFSET,
		                     MAX_OFFSET);
		Camera_->SetPosition(TransformComponent_->GetPosition() + glm::vec3(Rotation_ * glm::vec4(0, 0, -Offset_, 1)));
		Camera_->LookAt(TransformComponent_->GetPosition());

		// static constexpr float angle = glm::radians(35.0f);
		// glm::vec3 offset = glm::vec3(0, glm::sin(angle), -glm::cos(angle)) * Offset_;
		// offset = glm::mat3_cast(TransformComponent_->GetRotation()) * offset;
		// Camera_->SetPosition(TransformComponent_->GetPosition() + offset);
		// Camera_->LookAt(TransformComponent_->GetPosition());
	}
} // namespace dg::entity
