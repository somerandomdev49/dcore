#include <dcore/Misc/Terrain/Chunk.hpp>
#include <dcore/Renderer/Renderer.hpp>
using namespace dcore::terrain;

#define UNIT_PER_PIXEL 1

void Chunk::Initialize(HeightmapRegion &&region) { Region_ = std::move(region); }
void Chunk::DeInitialize() { DeActivate(); }

bool Chunk::IsActive() const { return IsActive_; }

void Chunk::Activate()
{
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

	// generating vertices
	auto regionSize = Region_.GetSize();
	for(int y = 0; y < regionSize.y; ++y)
		for(int x = 0; x < regionSize.x; ++x)
		{
			pushVec3(glm::vec3(x * UNIT_PER_PIXEL, 0, y * UNIT_PER_PIXEL)); // position
			pushVec3(glm::vec3(0, 1, 0)); // normal
			pushVec2(glm::vec2(0, 0)); // texcoord
		}

	// generating indices
	indices.reserve(regionSize.x * regionSize.y * 6);
	int vert = 0;
	for(int y = 0; y < regionSize.y; ++y)
	{

		for(int x = 0; x < regionSize.x; ++y)
		{
			indices.push_back(vert + 0);
			indices.push_back(vert + regionSize.x + 1);
			indices.push_back(vert + 1);
			indices.push_back(vert + 1);
			indices.push_back(vert + regionSize.x + 1);
			indices.push_back(vert + regionSize.x + 2);
			++vert;
		}
		++vert;
	}

	Mesh_ = new graphics::RStaticMesh();
	graphics::RenderResourceManager::CreateStaticMesh(Mesh_, indices, vertexData);
}
