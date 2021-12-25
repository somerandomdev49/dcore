#include <dg/Entity/CameraFollow.hpp>
#include <dcore/Platform/Platform.hpp>

DCORE_COMPONENT_REGISTER(dg::entity::CameraFollowComponent);

namespace dg::entity
{
	CameraFollowComponent::CameraFollowComponent(dcore::graphics::Camera *camera)
		: Camera_(camera)
	{
	}

	void CameraFollowComponent::Start(const dcore::world::EntityHandle &self)
	{
		TransformComponent_ = &dcore::world::Entity(self, dcore::platform::Context::Instance()->GetWorld())
			.GetComponent<dcore::world::TransformComponent>();
	}

	void CameraFollowComponent::Update(const dcore::world::EntityHandle &self)
	{
		glm::vec3 offset = glm::vec3(0, 0, 10); // TODO: Offset field + Getter/Setter for zooming in/out.
		// offset = glm::mat3_cast(TransformComponent_->GetRotation()) * offset;
		Camera_->SetPosition(TransformComponent_->GetPosition() + offset);
	}
}
