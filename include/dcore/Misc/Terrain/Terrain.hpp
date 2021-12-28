#pragma once
#include <dcore/Resource/ResourceManager.hpp>
#include <dcore/Misc/Terrain/Chunk.hpp>
#include <dcore/Core/Type.hpp>
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

		/** Activates chunks */
		void ActivateAllChunks();

		const Chunk &GetChunkAtGlobal(const glm::vec3 &position) const;

		const std::vector<Chunk> &GetChunks() const;
		const std::vector<dstd::UInt32> &GetActiveChunks() const;

		static float GetCChunkSize();
		static float GetCUnitsPerPixel();
		static float GetCHeight();

	private:
		void ActivateChunks_();

		std::vector<Chunk> Chunks_;
		std::vector<dstd::UInt32> ActiveChunks_;
		resource::Resource<Heightmap> Heightmap_;
		static float ChunkSize_;  // Chunk size in vertices
		static float UnitSize_;   // Size of a single pixel in units
		static float HeightMult_; // Y multiplier
	};

	class TerrainResourceManager
	{
	public:
		static void Register(resource::ResourceLoader *rl);
	};
} // namespace dcore::terrain
