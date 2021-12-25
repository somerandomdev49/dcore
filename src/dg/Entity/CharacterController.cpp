#include <dg/Entity/CharacterController.hpp>
#include <dcore/Event/InputManager.hpp>
#include <dcore/Event/TimeManager.hpp>
#include <dcore/Platform/Platform.hpp>

DCORE_COMPONENT_REGISTER(dg::entity::CharacterControllerComponent);

namespace dg::entity
{
	void CharacterControllerComponent::Start(const dcore::world::EntityHandle &self)
	{
		TransformComponent_ = &dcore::world::Entity(self, dcore::platform::Context::Instance()->GetWorld())
			.GetComponent<dcore::world::TransformComponent>();
	}

	void CharacterControllerComponent::Update(const dcore::world::EntityHandle &self)
	{
		auto inputMngr = dcore::event::InputManager::Instance();
		glm::vec2 movementVector = glm::vec2(0, 0);

		/**/ if(inputMngr->IsKeyPressed(dcore::event::K_W))
			movementVector.y = 1.0f;
		else if(inputMngr->IsKeyPressed(dcore::event::K_S))
			movementVector.y = -1.0f;
		/**/ if(inputMngr->IsKeyPressed(dcore::event::K_D))
			movementVector.x = +1.0f;
		else if(inputMngr->IsKeyPressed(dcore::event::K_A))
			movementVector.x = -1.0f;

		// movementVector = glm::normalize(movementVector);
		glm::vec3 v = glm::vec3(movementVector.x, 0, movementVector.y);
		// v = glm::mat3_cast(TransformComponent_->GetRotation()) * v;

		// Velocity_ += glm::vec3(0, -Gravity_ * 0, 0);

		auto position = TransformComponent_->GetPosition();
		position += v * Speed_ * dcore::event::TimeManager::Instance()->GetDeltaTime();
		// position += Velocity_ * dcore::event::TimeManager::Instance()->GetDeltaTime();
		TransformComponent_->SetPosition(position);
		// fprintf(stderr, "pos: %f, %f, %f\n", position.x, position.y, position.z);
	}
}
