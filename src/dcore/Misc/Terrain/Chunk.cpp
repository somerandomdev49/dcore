#include <dcore/Misc/Terrain/Chunk.hpp>
#include <dcore/Renderer/Renderer.hpp>
#include <dcore/Core/Log.hpp>
using namespace dcore::terrain;

#define UNIT_PER_PIXEL 1

inline std::ostream &operator<<(std::ostream &os, const glm::ivec2 &v)
{
	os << "(" << v.x << ", " << v.y << ")";
	return os;
}

Chunk::Chunk(HeightmapRegion &&region, const glm::ivec2 &localPosition) : Region_(std::move(region)), LocalPosition_(localPosition) {}
void Chunk::Initialize() { IsActive_ = false; }
void Chunk::DeInitialize()
{
	if(IsActive_) DeActivate();
}

bool Chunk::IsActive() const { return IsActive_; }

void Chunk::Activate()
{
	if(IsActive_) return;
	IsActive_ = true;
	GenerateMesh_();
}

void Chunk::DeActivate()
{
	IsActive_ = false;
	graphics::RenderResourceManager::DeleteStaticMesh(Mesh_);
	delete Mesh_;
}

void Chunk::GenerateMesh_()
{
	std::vector<uint32_t> indices;
	std::vector<uint8_t> vertexData;

	const auto pushFloat = [&](float f) {
		uint8_t *bytes = reinterpret_cast<uint8_t *>(&f);
		vertexData.push_back(bytes[0]);
		vertexData.push_back(bytes[1]);
		vertexData.push_back(bytes[2]);
		vertexData.push_back(bytes[3]);
	};

	const auto pushVec2 = [&](const glm::vec2 &v) {
		pushFloat(v.x);
		pushFloat(v.y);
	};

	const auto pushVec3 = [&](const glm::vec3 &v) {
		pushFloat(v.x);
		pushFloat(v.y);
		pushFloat(v.z);
	};

	// pushVec3(glm::vec3(0, 1, -1)); pushVec3(glm::vec3(0, 1, 0)); pushVec2(glm::vec2(0, 0));
	// pushVec3(glm::vec3(1, -1, -1)); pushVec3(glm::vec3(0, 1, 0)); pushVec2(glm::vec2(0, 0));
	// pushVec3(glm::vec3(1, 1, -1)); pushVec3(glm::vec3(0, 1, 0)); pushVec2(glm::vec2(0, 0));

	// indices.push_back(0);
	// indices.push_back(1);
	// indices.push_back(2);

	// generating vertices
	int vertexCount = 0;
	auto regionSize = Region_.GetSize() + glm::ivec2(1, 1);
	// DCORE_LOG_INFO << "Region Size: " << regionSize;
	for(int y = 0; y < regionSize.y; ++y)
		for(int x = 0; x < regionSize.x; ++x)
		{
			float h = Region_.Get(glm::ivec2(x, y));
			pushVec3(glm::vec3(x * UNIT_PER_PIXEL - CHUNK_SIZE / 2, h, y * UNIT_PER_PIXEL - CHUNK_SIZE / 2)); // position
			pushVec3(glm::vec3(0, 1, 0));                                                                     // normal
			pushVec2(glm::vec2(0, 0));                                                                        // texcoord
			++vertexCount;
		}

	// generating indices
	// DCORE_LOG_INFO << "Index Count: " << regionSize.x * regionSize.y * 6;
	indices.reserve(regionSize.x * (regionSize.y - 1) * 6);
	int vert = 0;
	for(int y = 0; y < regionSize.y - 1; ++y)
	{
		for(int x = 0; x < regionSize.x; ++x)
		{
			if(x == regionSize.x - 1)
			{
				++vert;
				continue;
			}
			indices.push_back(vert + 0);
			indices.push_back(vert + 1);
			indices.push_back(vert + regionSize.x);

			indices.push_back(vert + 1);
			indices.push_back(vert + regionSize.x + 1);
			indices.push_back(vert + regionSize.x);
			++vert;
		}
	}
	indices.shrink_to_fit();

	// DCORE_LOG_INFO << "Done generating chunk. Region Size: " << regionSize << ", Vertex Count: " << vertexCount << ", Index Count: " <<
	indices.size();

	// size_t vertexSize = sizeof(float) * (3 + 3 + 2);
	// DCORE_LOG_INFO << "Index Count: " << indices.size() << ", Vertex Count: " << (float)vertexData.size() / (float)vertexSize;

	Mesh_ = new graphics::RStaticMesh();
	graphics::RenderResourceManager::CreateStaticMesh(Mesh_, indices, vertexData);
}

const dcore::resource::Resource<dcore::graphics::RTexture> &Chunk::GetBlendMap() const { return BlendMap_; }
const dcore::resource::Resource<dcore::graphics::RTexture> *Chunk::GetTextures() const { return &Textures_[0]; }

glm::vec2 Chunk::GetGlobalPosition() const { return LocalPosition_ * UNIT_PER_PIXEL; }
const glm::ivec2 &Chunk::GetLocalPosition() const { return LocalPosition_; }

dcore::graphics::RStaticMesh *Chunk::GetMesh() const { return Mesh_; }
