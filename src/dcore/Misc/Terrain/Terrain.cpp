#include <dcore/Misc/Terrain/Terrain.hpp>

using namespace dcore::terrain;

const std::vector<uint32_t> &Terrain::GetActiveChunks() const { return ActiveChunks_; }
const std::vector<Chunk> &Terrain::GetChunks() const { return Chunks_; }

void Terrain::ReactivateChunks(const glm::vec3 &position, float radius)
{
	// TODO: Reserve active chunks! (there is a formula for fitting squares in a circle)
	for(uint32_t i = 0; i < Chunks_.size(); ++i)
	{
		auto &localPos = Chunks_[i].GetGlobalPosition();
		if(glm::distance(glm::vec3(localPos.x, 0, localPos.y), position) < radius)
			ActiveChunks_.push_back(i);
	}
}

void TerrainResourceManager::Register(resource::ResourceLoader *rl)
{
	resource::ResourceManager::Instance()->RegisterConstructor<Heightmap>(&Heightmap::Heightmap_Constructor);
	resource::ResourceManager::Instance()->RegisterDeConstructor<Heightmap>(&Heightmap::Heightmap_DeConstructor);
	rl->RegisterResourceType<Heightmap>("Heightmap");
}
