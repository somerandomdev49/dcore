#pragma once
#include <dcore/Physics/PhysicsWorld.hpp>
#include <dcore/Misc/Terrain/Terrain.hpp>
#include <glm/glm.hpp>

namespace dcore::physics
{
	class TerrainCollider
	{
	public:
		TerrainCollider(terrain::Chunk *chunk);

	private:
		terrain::Chunk *Chunk_;
	};
} // namespace dcore::physics
