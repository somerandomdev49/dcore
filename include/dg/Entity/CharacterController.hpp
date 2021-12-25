#include <dcore/World/Entity.hpp>
#include <dcore/World/World.hpp>
#include <glm/glm.hpp>

namespace dg::entity
{
	class CharacterControllerComponent : public dcore::world::ComponentBase<CharacterControllerComponent>
	{
	public:
		void Start(const dcore::world::EntityHandle &self);
		void Update(const dcore::world::EntityHandle &self);
	private:
		dcore::world::TransformComponent *TransformComponent_ = nullptr;

		float Speed_ = 20.0f;
		float Gravity_ = 10.0f;
		glm::vec3 Velocity_ = glm::vec3(0, 0, 0);
	};
}
