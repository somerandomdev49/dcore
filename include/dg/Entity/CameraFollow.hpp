#pragma once
#include <dcore/World/Entity.hpp>
#include <dcore/World/World.hpp>

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
		dcore::world::TransformComponent *TransformComponent_ = nullptr;
		dcore::graphics::Camera *Camera_ = nullptr;
		float Offset_ = DefaultOffset_;
		glm::vec2 Last_ = {0,0}, Total_ = {0, 0};
		glm::mat4 Rotation_ = glm::mat4(1);
	};
} // namespace dg::entity
