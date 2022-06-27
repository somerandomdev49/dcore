#include "dcore/Renderer/RSkyBox.hpp"
#include "dcore/Resource/ResourceManager.hpp"
#include "dcore/World/World.hpp"
#include <dg/Entity/CharacterController.hpp>
#include <dcore/Event/InputManager.hpp>
#include <dcore/Event/TimeManager.hpp>
#include <dcore/Platform/Platform.hpp>
#include <dcore/Core/FrameLog.hpp>

DCORE_COMPONENT_REGISTER(dg::entity::CharacterControllerComponent);
namespace
{
	glm::quat FromEulerZYX(float pitch, float yaw, float roll)
	{
		// https://gamedev.stackexchange.com/questions/13436/glm-euler-angles-to-quaternion
		float sx = sin(pitch / 2), sy = sin(yaw / 2), sz = sin(roll / 2);
		float cx = cos(pitch / 2), cy = cos(yaw / 2), cz = cos(roll / 2);

		return {
		    cx * cy * cz - sx * sy * sz,
		    sx * cy * cz + cx * sy * sz,
		    cx * sy * cz - sx * cy * sz,
		    cx * cy * sz + sx * sy * cz,
		};
	}
} // namespace

namespace dg::entity
{
	void CharacterControllerComponent::Start(const dcore::world::EntityHandle &self)
	{
		TransformComponent_ = dcore::world::Entity(self, dcore::platform::Context::Instance()->GetWorld())
		                          .GetComponent<dcore::world::TransformComponent>();
	}

	void CharacterControllerComponent::Update(const dcore::world::EntityHandle &self)
	{
		auto *inputMngr          = dcore::event::InputManager::Instance();
		glm::vec2 movementVector = glm::vec2(0, 0);

		if(inputMngr->IsKeyPressed(dcore::event::K_W)) movementVector.y = 1.0f;
		else if(inputMngr->IsKeyPressed(dcore::event::K_S))
			movementVector.y = -1.0f;

		if(inputMngr->IsKeyPressed(dcore::event::K_D)) movementVector.x = -1.0f;
		else if(inputMngr->IsKeyPressed(dcore::event::K_A))
			movementVector.x = +1.0f;

		IsMoving_ = (movementVector != glm::vec2(0, 0));

		if(movementVector.x != 0 && movementVector.y != 0) movementVector = glm::normalize(movementVector);
		glm::vec3 vel =
		    glm::rotate(glm::mat4(1), Yaw_, {0, 1, 0}) * glm::vec4(movementVector.x, 0, movementVector.y, 1);
		TransformComponent_->SetRotation(FromEulerZYX(0, Yaw_, 0));
		// v = glm::mat3_cast(TransformComponent_->GetRotation()) * v;

		// Velocity_ += glm::vec3(0, -Gravity_ * 0, 0);

		auto position = TransformComponent_->GetPosition();

		const dcore::terrain::Chunk &currentChunk =
		    dcore::platform::Context::Instance()->GetWorld()->GetTerrain()->GetChunkAtGlobal(position);

		position += vel * Speed_ * dcore::event::TimeManager::Instance()->GetDeltaTime();
		// // position += Velocity_ * dcore::event::TimeManager::Instance()->GetDeltaTime();

		float terrainHeight = currentChunk.GetHeightAtGlobal(glm::vec2(position.x, position.z));
		// if(position.y < terrainHeight)
		position.y = terrainHeight + 1.0f /* TODO: Half-height of the capsule? */;

		TransformComponent_->SetPosition(position);
		dcore::FrameLog::SLogF("pos: %.2f, %.2f, %.2f", position.x, position.y, position.z);
		dcore::FrameLog::SLogF("chunk: %d, %d", currentChunk.GetLocalPosition().x, currentChunk.GetLocalPosition().y);

		if(inputMngr->IsKeyPressed(dcore::event::K_Q))
		{
			auto *skybox = dcore::resource::GetResource<dcore::graphics::RSkyBox>("DCore.SkyBox.Main.Kasharok").Get();
			dcore::platform::Context::Instance()->GetWorld()->SetTargetSkyBox(skybox);
		}
	}
} // namespace dg::entity
