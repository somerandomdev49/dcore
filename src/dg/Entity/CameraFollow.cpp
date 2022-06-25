#include "dcore/Event/TimeManager.hpp"
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
	// TODO: Place in header, same for CharacterController.cpp
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
		auto entity = dcore::world::Entity(self, dcore::platform::Context::Instance()->GetWorld());
		Transform_ = entity.GetComponent<dcore::world::TransformComponent>();
		Character_ = entity.GetComponent<dg::entity::CharacterControllerComponent>();

		if(Transform_ == nullptr)
			LOG_F(ERROR, "CameraFollowComponent's entity has to have a TransformComponent!");
		
		if(Character_ == nullptr)
			LOG_F(ERROR, "CharacterControllerComponent's entity has to have a TransformComponent!");
		
		// Camera_->SetPosition(TransformComponent_->GetPosition() + glm::vec3(0, 0, Offset_));
	}

	void CameraFollowComponent::UpdateCameraRotation_(float xAngle, float yAngle)
	{

#define EPSILON 0.99f
		Total_.x += xAngle;
		Total_.y -= yAngle;
		Total_.y = glm::clamp(Total_.y, -glm::pi<float>() / 3, glm::pi<float>() / 3);

		Rotation_ = glm::rotate(glm::mat4(1), Total_.x, glm::vec3(0, 1, 0));
		Rotation_ = glm::rotate(Rotation_, Total_.y, glm::vec3(1, 0, 0));
	}

	void CameraFollowComponent::Update(dcore::world::EntityHandle self)
	{
		auto *frame = dcore::platform::Context::Instance()->GetFrame();
		bool lmb = dcore::event::InputManager::Instance()->IsMousePressed(0),
		     rmb = dcore::event::InputManager::Instance()->IsMousePressed(1);
		bool shouldRotate = lmb || rmb;
		
		if(shouldRotate)
		{
			CameraMode_ = 0;
			Timer_ = 0;

			frame->SetCursorState(dcore::platform::Frame::CursorState_Grab);
			
			auto mousePos = dcore::event::InputManager::Instance()->GetMousePosition();

			auto frameSize = dcore::platform::Context::Instance()->GetFrame()->GetSize();
			float deltaAngleX = 2 * glm::pi<float>() / frameSize.x;
			float deltaAngleY = glm::pi<float>() / frameSize.y;
			float xAngle = (LastMousePos_.x - mousePos.x) * deltaAngleX;
			float yAngle = (LastMousePos_.y - mousePos.y) * deltaAngleY;
			LastMousePos_ = mousePos;
			UpdateCameraRotation_(xAngle, yAngle);

			if(lmb && Character_->IsMoving())
				Character_->SetYaw(Total_.x);
		}
		else
		{
#define CAMERA_FREELOOK_WAIT 2.50f
#define CAMERA_MOVE_SPEED 0.05f
			if(Character_->IsMoving())
			{
				switch(CameraMode_)
				{
				case 3:
					Character_->SetYaw(Total_.x);
					CameraMode_ = 0;
					[[fallthrough]];
				case 0:
					if(Timer_ >= CAMERA_FREELOOK_WAIT)
						CameraMode_ = 1;
					else 
						Timer_ += dcore::event::TimeManager::Instance()
							->GetDeltaTime();
					break;
				case 1:
					// float deltaR = angles.x + Total_.x;
					// if(glm::abs(deltaL) > glm::abs(deltaR))
					// 	deltaL = deltaR;
					float delta = Character_->GetYaw() - Total_.x;
					Total_.x += delta * CAMERA_MOVE_SPEED;
					dcore::FrameLog::Instance()->LogF("delta: %f", delta);
					UpdateCameraRotation_(0, 0);
					break;
				}
			}
			else
			{
				Timer_ = 0;
				CameraMode_ = 3;
			}
			frame->SetCursorState(dcore::platform::Frame::CursorState_Normal);
			LastMousePos_ = dcore::event::InputManager::Instance()->GetMousePosition();
		}

		
		Offset_ = glm::clamp(
			Offset_ - dcore::event::InputManager::Instance()->GetScrollY() * SCROLL_SPEED,
			MIN_OFFSET,
			MAX_OFFSET
		);
		Camera_->SetPosition(Transform_->GetPosition() + glm::vec3(Rotation_ * glm::vec4(0, 0, -Offset_, 1)));
		Camera_->LookAt(Transform_->GetPosition());

		// static constexpr float angle = glm::radians(35.0f);
		// glm::vec3 offset = glm::vec3(0, glm::sin(angle), -glm::cos(angle)) * Offset_;
		// offset = glm::mat3_cast(TransformComponent_->GetRotation()) * offset;
		// Camera_->SetPosition(TransformComponent_->GetPosition() + offset);
		// Camera_->LookAt(TransformComponent_->GetPosition());
	}
} // namespace dg::entity
