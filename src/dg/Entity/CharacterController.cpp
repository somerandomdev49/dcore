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
		auto *inputMngr = dcore::event::InputManager::Instance();

		glm::vec2 movement = { 0.0f, 0.0f };
		if(inputMngr->IsKeyPressed(dcore::event::K_W))
				movement += glm::vec2 { 0.0f, +1.0f };
		else if(inputMngr->IsKeyPressed(dcore::event::K_S))
				movement += glm::vec2 { 0.0f, -1.0f };

		if(inputMngr->IsKeyPressed(dcore::event::K_D))
			movement += glm::vec2 { -1.0f, 0.0f };
		else if(inputMngr->IsKeyPressed(dcore::event::K_A))
			movement += glm::vec2 { +1.0f, 0.0f };

		if(movement != glm::vec2(0.0f, 0.0f))
			Movement_ = movement;

		IsMoving_ = (Movement_ != glm::vec2(0, 0));
		if(Movement_.x != 0 && Movement_.y != 0) Movement_ = glm::normalize(Movement_);

		glm::vec3 vel =
		    glm::rotate(glm::mat4(1), Yaw_, {0, 1, 0}) * glm::vec4(Movement_.x, 0, Movement_.y, 1);
		TransformComponent_->SetRotation(FromEulerZYX(0, Yaw_, 0));

		auto position = TransformComponent_->GetPosition();
		position += vel * Speed_ * dcore::event::TimeManager::Instance()->GetDeltaTime();
	
		const dcore::terrain::Chunk &currentChunk =
		    dcore::platform::Context::Instance()->GetWorld()->GetTerrain()->GetChunkAtGlobal(position);
		float terrainHeight = currentChunk.GetHeightAtGlobal(glm::vec2(position.x, position.z));
		
		position.y = terrainHeight + 1.0f;
		TransformComponent_->SetPosition(position);
		
		dcore::FrameLog::SLogF("pos: %.2f, %.2f, %.2f", position.x, position.y, position.z);
		dcore::FrameLog::SLogF("chunk: %d, %d", currentChunk.GetLocalPosition().x, currentChunk.GetLocalPosition().y);

		Movement_ = glm::vec2(0, 0);
	}
} // namespace dg::entity
