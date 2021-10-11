#pragma once
#include <dcore/Game/Terrain/Chunk.hpp>
#include <dcore/Resource/ResourceManager.hpp>

namespace dg::terrain {
	class Terrain {
	public:
		/** Recalculates which chunks need to be active based on a position and a radius */
		void RemapChunks(const glm::vec3 &position, float radius = 100.f);

		const std::vector<Chunk> &GetChunks() const;
		const std::vector<long> &GetActiveChunks() const;

	private:
		std::vector<Chunk> Chunks_;
		std::vector<long> ActiveChunks_;
	};
} // namespace dg::terrain
