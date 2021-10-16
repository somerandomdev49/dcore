#pragma once
#include <dcore/Resource/ResourceManager.hpp>
#include <dcore/Misc/Terrain/Chunk.hpp>
#include <dcore/Uni.hpp>
#include <glm/glm.hpp>
#include <vector>

namespace dcore::terrain
{
	class Terrain
	{
	public:
		void Initialize(const resource::Resource<Heightmap> &heightmap);
		void DeInitialize();

		/** Recalculates which chunks need to be active based on a position and a radius */
		void ReactivateChunks(const glm::vec3 &position, float radius = 100.f);

		const std::vector<Chunk> &GetChunks() const;
		const std::vector<uint32_t> &GetActiveChunks() const;

	private:
		std::vector<Chunk> Chunks_;
		std::vector<uint32_t> ActiveChunks_;
		resource::Resource<Heightmap> Heightmap_;
	};

	class TerrainResourceManager
	{
	public:
		static void Register(resource::ResourceLoader *rl);
	};
} // namespace dcore::terrain
