#pragma once
#include <glm/glm.hpp>
#include <dcore/Resource/ResourceManager.hpp>

namespace dcore::terrain
{
	class Heightmap
	{
	public:
		/** Returns the max height of the terrain. (255) */
		uint32_t GetMaxHeight() const;

		/** Returns the min height of the terrain. (0) */
		uint32_t GetMinHeight() const;

		/** Returns the height at a specified point in a texture. */
		float Get(const glm::ivec2 &pos) const;

		/** Returns the size of the heightmap. */
		const glm::ivec2 &GetSize() const;

	private:
		friend class TerrainResourceManager;
		static void Heightmap_Constructor(const std::string &path, void *placement);
		static void Heightmap_DeConstructor(void *placement);

		glm::ivec2 Size_;
		uint32_t *Data_;
	};

	class HeightmapRegion
	{
	public:
		HeightmapRegion(Heightmap *source, const glm::ivec2 &min, const glm::ivec2 &max);

		/** Returns the minimum xy of the rectangle region */
		const glm::ivec2 &GetMin() const;

		/** Returns the maximum xy of the rectangle region */
		const glm::ivec2 &GetMax() const;

		/** Returns the size of the region (GetMax() - GetMin()) */
		glm::ivec2 GetSize() const;

		/** Returns the source heightmap */
		Heightmap *GetSource();

		/** Returns the pixel value at a location. */
		float Get(const glm::ivec2 &pos) const;

	private:
		glm::ivec2 Min_, Max_;
		Heightmap *Source_;
	};
} // namespace dcore::terrain
