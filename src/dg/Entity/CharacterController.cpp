#include <dg/Entity/CharacterController.hpp>
#include <dcore/Event/InputManager.hpp>
#include <dcore/Event/TimeManager.hpp>
#include <dcore/Platform/Platform.hpp>
#include <dcore/Core/FrameLog.hpp>

DCORE_COMPONENT_REGISTER(dg::entity::CharacterControllerComponent);

namespace dg::entity
{
	void CharacterControllerComponent::Start(const dcore::world::EntityHandle &self)
	{
		TransformComponent_ = dcore::world::Entity(self, dcore::platform::Context::Instance()->GetWorld())
		                          .GetComponent<dcore::world::TransformComponent>();
	}

	void CharacterControllerComponent::Update(const dcore::world::EntityHandle &self)
	{
		auto inputMngr           = dcore::event::InputManager::Instance();
		glm::vec2 movementVector = glm::vec2(0, 0);

		/**/ if(inputMngr->IsKeyPressed(dcore::event::K_W))
			movementVector.y = 1.0f;
		else if(inputMngr->IsKeyPressed(dcore::event::K_S)) movementVector.y = -1.0f;
		/**/ if(inputMngr->IsKeyPressed(dcore::event::K_D))
			movementVector.x = -1.0f;
		else if(inputMngr->IsKeyPressed(dcore::event::K_A)) movementVector.x = +1.0f;

		// movementVector = glm::normalize(movementVector);
		glm::vec3 v = glm::vec3(movementVector.x, 0, movementVector.y);
		// v = glm::mat3_cast(TransformComponent_->GetRotation()) * v;

		// Velocity_ += glm::vec3(0, -Gravity_ * 0, 0);

		auto position = TransformComponent_->GetPosition();

		const dcore::terrain::Chunk &currentChunk =
		    dcore::platform::Context::Instance()->GetWorld()->GetTerrain()->GetChunkAtGlobal(position);

		position += v * Speed_ * dcore::event::TimeManager::Instance()->GetDeltaTime();
		// position += Velocity_ * dcore::event::TimeManager::Instance()->GetDeltaTime();

		float terrainHeight = currentChunk.GetHeightAtGlobal(glm::vec2(position.x, position.z));
		// if(position.y < terrainHeight)
		position.y = terrainHeight + 1.0f /* TODO: Half-height of the capsule? */;

		TransformComponent_->SetPosition(position);
		dcore::FrameLog::SLogF("pos: %.2f, %.2f, %.2f", position.x, position.y, position.z);
		dcore::FrameLog::SLogF("chunk: %d, %d", currentChunk.GetLocalPosition().x, currentChunk.GetLocalPosition().y);
	}
} // namespace dg::entity
