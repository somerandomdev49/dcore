#include "glm/gtc/type_ptr.hpp"
#include "glm/trigonometric.hpp"
#include <dg/Entity/CameraFollow.hpp>

#include <dcore/Core/FrameLog.hpp>
#include <dcore/Platform/Platform.hpp>

DCORE_COMPONENT_REGISTER(dg::entity::CameraFollowComponent);
#define MAX_OFFSET   40.0f
#define MIN_OFFSET   5.0f
#define SCROLL_SPEED 1.5f // TODO: Add to preferences.

namespace dg::entity
{
	CameraFollowComponent::CameraFollowComponent(dcore::graphics::Camera *camera) : Camera_(camera) {}

	void CameraFollowComponent::Start(dcore::world::EntityHandle self)
	{
		TransformComponent_ = dcore::world::Entity(self, dcore::platform::Context::Instance()->GetWorld())
		                          .GetComponent<dcore::world::TransformComponent>();

		if(TransformComponent_ == nullptr)
			LOG_F(ERROR, "CameraFollowComponent's entity has to have a TransformComponent!");
	}

	void CameraFollowComponent::Update(dcore::world::EntityHandle self)
	{
		if(TransformComponent_ == nullptr) return;

		Offset_ = glm::clamp(Offset_ - dcore::event::InputManager::Instance()->GetScrollY() * SCROLL_SPEED, MIN_OFFSET,
		                     MAX_OFFSET);

		static constexpr float angle = glm::radians(35.0f);

		glm::vec3 offset = glm::vec3(0, glm::sin(angle), -glm::cos(angle)) * Offset_;
		// offset = glm::mat3_cast(TransformComponent_->GetRotation()) * offset;
		Camera_->SetPosition(TransformComponent_->GetPosition() + offset);
		Camera_->LookAt(TransformComponent_->GetPosition());
	}
} // namespace dg::entity
