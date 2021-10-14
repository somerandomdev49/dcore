#pragma once
#include <dcore/Resource/ResourceManager.hpp>
#include <dcore/Misc/Terrain/Chunk.hpp>
#include <glm/glm.hpp>
#include <vector>

namespace dcore::terrain
{
	class Terrain
	{
	public:
		/** Recalculates which chunks need to be active based on a position and a radius */
		void RemapChunks(const glm::vec3 &position, float radius = 100.f);

		const std::vector<Chunk> &GetChunks() const;
		const std::vector<long> &GetActiveChunks() const;

	private:
		std::vector<Chunk> Chunks_;
		std::vector<long> ActiveChunks_;
	};
} // namespace dcore::terrain
