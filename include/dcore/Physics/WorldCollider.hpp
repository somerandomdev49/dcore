#pragma once
#include <dcore/Physics/PhysicsWorld.hpp>
#include <glm/glm.hpp>
#include <vector>

namespace dcore::physics
{
	class WorldSection
	{
	private:
		// Whole structure offset.
		glm::vec3 Offset_;

		// XZ polygon points.
		std::vector<glm::vec2> Points_;

		// Y component for each of the points.
		float Height_;
	};

	class WorldCollider : public Collider
	{
	public:
		WorldCollider(WorldSection &&section);
	private:
		WorldSection Section_;
	};
} // namespace dcore::physics
