#include <dg/Entity/CameraFollow.hpp>

#include <dcore/Core/FrameLog.hpp>
#include <dcore/Platform/Platform.hpp>

DCORE_COMPONENT_REGISTER(dg::entity::CameraFollowComponent);

namespace dg::entity
{
	CameraFollowComponent::CameraFollowComponent(dcore::graphics::Camera *camera) : Camera_(camera) {}

	void CameraFollowComponent::Start(const dcore::world::EntityHandle &self)
	{
		TransformComponent_ = dcore::world::Entity(self, dcore::platform::Context::Instance()->GetWorld())
		                          .GetComponent<dcore::world::TransformComponent>();

		if(TransformComponent_ == nullptr)
			LOG_F(ERROR, "CameraFollowComponent's entity has to have a TransformComponent!");
	}

	void CameraFollowComponent::Update(const dcore::world::EntityHandle &self)
	{
		LOG_F(INFO, "Camera Follow | Update");
		if(TransformComponent_ == nullptr) return;

		glm::vec3 offset = glm::vec3(0, 10, -40); // TODO: Offset field + Getter/Setter for zooming in/out.
		// offset = glm::mat3_cast(TransformComponent_->GetRotation()) * offset;
		Camera_->SetPosition(TransformComponent_->GetPosition() + offset);
		Camera_->LookAt(TransformComponent_->GetPosition());
	}
} // namespace dg::entity
