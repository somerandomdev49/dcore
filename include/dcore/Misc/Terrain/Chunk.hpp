#pragma once
#include <dcore/Resource/ResourceLoader.hpp>
#include <dcore/Misc/Terrain/Heightmap.hpp>
#include <dcore/Renderer/RStaticMesh.hpp>
#include <dcore/Renderer/RTexture.hpp>
#define CHUNK_SIZE 16

namespace dcore::terrain
{
	class Chunk
	{
	public:
		Chunk(HeightmapRegion &&region, const glm::ivec2 &localPosition);
		void Initialize();
		void DeInitialize();

		/** Returns the region the chunk is based on. */
		const HeightmapRegion &GetRegion() const;

		/** Returns the size of the chunk. (region size * resolution) */
		const glm::vec2 &GetSize() const;

		/** Returns the actual location of the chunk. */
		glm::vec2 GetGlobalPosition() const;

		/** Returns the index of the chunk. */
		const glm::ivec2 &GetLocalPosition() const;

		/** Returns whether the chunk is active. (can be rendered) */
		bool IsActive() const;

		const dcore::resource::Resource<dcore::graphics::RTexture> &GetBlendMap() const;
		const dcore::resource::Resource<dcore::graphics::RTexture> *GetTextures() const;
		dcore::graphics::RStaticMesh *GetMesh() const;

	private:
		friend class Terrain;

		/** Activates the chunk and creates the mesh. */
		void Activate();

		/** Deactivates the chunk and deletes the mesh. */
		void DeActivate();

		/** Private function for generating the mesh of the chunk. Called by Activate() */
		void GenerateMesh_();

		HeightmapRegion Region_;

		dcore::resource::Resource<dcore::graphics::RTexture> BlendMap_;
		dcore::resource::Resource<dcore::graphics::RTexture> Textures_[4];

		dcore::graphics::RStaticMesh DCORE_OWN *Mesh_;
		glm::ivec2 LocalPosition_;
		bool IsActive_;
	};
} // namespace dcore::terrain
