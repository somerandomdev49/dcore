#include <dcore/Misc/Terrain/Terrain.hpp>

using namespace dcore::terrain;

#define CHUNK_SIZE 50

void Terrain::Initialize(const resource::Resource<Heightmap> &heightmap)
{
	Heightmap_ = heightmap;

	glm::ivec2 chunkCount = Heightmap_.Get()->GetSize() / glm::ivec2(CHUNK_SIZE, CHUNK_SIZE);
	Chunks_.reserve(chunkCount.x * chunkCount.y);

	for(int y = 0; y < chunkCount.y; ++y)
		for(int x = 0; x < chunkCount.x; ++x)
		{
			glm::ivec2 pos(x, y);
			Chunks_.push_back(Chunk(HeightmapRegion(Heightmap_.Get(), pos, pos)));
		}
}

void Terrain::DeInitialize()
{
	for(auto &c : Chunks_) c.DeInitialize();
}

const std::vector<uint32_t> &Terrain::GetActiveChunks() const { return ActiveChunks_; }
const std::vector<Chunk> &Terrain::GetChunks() const { return Chunks_; }

void Terrain::ReactivateChunks(const glm::vec3 &position, float radius)
{
	// TODO: Reserve active chunks! (there is a formula for fitting squares in a circle)
	for(uint32_t i = 0; i < Chunks_.size(); ++i)
	{
		auto pos = Chunks_[i].GetGlobalPosition();
		if(glm::distance(glm::vec3(pos.x, 0, pos.y), position) < radius) ActiveChunks_.push_back(i);
	}
}

void TerrainResourceManager::Register(resource::ResourceLoader *rl)
{
	resource::ResourceManager::Instance()->RegisterConstructor<Heightmap>(&Heightmap::Heightmap_Constructor);
	resource::ResourceManager::Instance()->RegisterDeConstructor<Heightmap>(&Heightmap::Heightmap_DeConstructor);
	rl->RegisterResourceType<Heightmap>("Heightmap");
}
