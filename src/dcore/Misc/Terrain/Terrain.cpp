#include <dcore/Misc/Terrain/Terrain.hpp>
#include <dcore/Core/Log.hpp>

namespace glm
{
	inline std::ostream &operator<<(std::ostream &os, const ivec2 &v)
	{
		os << "(" << v.x << ", " << v.y << ")";
		return os;
	}
} // namespace glm

namespace dcore::terrain
{
	dstd::UInt32 Terrain::ChunkSize_ = 15;
	float Terrain::UnitSize_         = 1;
	float Terrain::HeightMult_       = 45;

	void Terrain::Initialize(const resource::Resource<Heightmap> &heightmap)
	{
		Heightmap_ = heightmap;

		glm::ivec2 chunkCount = Heightmap_.Get()->GetSize() / glm::ivec2(ChunkSize_);
		DCORE_LOG_INFO << "Initializing terrain: Heightmap Size: " << Heightmap_.Get()->GetSize()
		               << ", Chunk Count: " << chunkCount;
		Chunks_.reserve(chunkCount.x * chunkCount.y);

		auto texture = resource::ResourceManager::Instance()->Get<graphics::RTexture>(
		    "DCore.Texture.Main.Grass");
		for(int y = 0; y < chunkCount.y; ++y)
			for(int x = 0; x < chunkCount.x; ++x)
			{
				glm::ivec2 pos(x * ChunkSize_, y * ChunkSize_);
				Chunks_.push_back(Chunk(
				    HeightmapRegion(Heightmap_.Get(), pos, pos + glm::ivec2(ChunkSize_)), pos));
				Chunks_[Chunks_.size() - 1].Initialize();
				Chunks_[Chunks_.size() - 1].SetTexture(0, texture);
			}
	}
	
	dstd::UInt32 Terrain::GetCChunkSize() { return ChunkSize_; }
	float Terrain::GetCUnitsPerPixel() { return UnitSize_; }
	float Terrain::GetCHeight() { return HeightMult_; }

	void Terrain::DeInitialize()
	{
		for(auto &c : Chunks_) c.DeInitialize();
	}

	const std::vector<uint32_t> &Terrain::GetActiveChunks() const { return ActiveChunks_; }
	const std::vector<Chunk> &Terrain::GetChunks() const { return Chunks_; }

	void Terrain::ActivateChunks_()
	{
		for(uint32_t idx : ActiveChunks_) Chunks_[idx].Activate();
	}

	void Terrain::ActivateAllChunks()
	{
		ActiveChunks_.clear();
		for(uint32_t i = 0; i < Chunks_.size(); ++i) ActiveChunks_.push_back(i);
		ActivateChunks_();
	}

	void Terrain::ReactivateChunks(const glm::vec3 &position, float radius)
	{
		ActiveChunks_.clear();
		// TODO: Reserve active chunks! (there is a formula for fitting squares in a circle)
		for(uint32_t i = 0; i < Chunks_.size(); ++i)
		{
			auto pos = Chunks_[i].GetGlobalPosition();
			if(glm::distance(glm::vec3(pos.x, 0, pos.y), position) < radius)
				ActiveChunks_.push_back(i);
		}
		ActivateChunks_();
	}

	const Chunk &Terrain::GetChunkAtGlobal(const glm::vec3 &position) const
	{
		auto chunkCount    = Heightmap_.Get()->GetSize() / glm::ivec2(ChunkSize_);
		glm::ivec2 gridPos = glm::vec2(position.x, position.z) / (float)ChunkSize_;
		return Chunks_[gridPos.x + gridPos.y * chunkCount.x];
	}

	void TerrainResourceManager::Register(resource::ResourceLoader *rl)
	{
		resource::ResourceManager::Instance()->RegisterConstructor<Heightmap>(
		    &Heightmap::Heightmap_Constructor);
		resource::ResourceManager::Instance()->RegisterDeConstructor<Heightmap>(
		    &Heightmap::Heightmap_DeConstructor);
		rl->RegisterResourceType<Heightmap>("Heightmap");
	}
} // namespace dcore::terrain
