#include <dcore/Misc/Terrain/Chunk.hpp>
#include <dcore/Renderer/Renderer.hpp>
using namespace dcore::terrain;

#define UNIT_PER_PIXEL 1 // TODO: implement

void Chunk::Initialize(HeightmapRegion &&region) { Region_ = std::move(region); }
void Chunk::DeInitialize() {}

bool Chunk::IsActive() const { return IsActive_; }

void Chunk::Activate()
{
	IsActive_ = true;
	GenerateMesh_();
}

void Chunk::DeActivate()
{
	// ...
	graphics::RenderResourceManager::DeleteStaticMesh(Mesh_);
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

	const auto pushFloats = [&](auto... vs) {
		pushFloat(vs)...;
	};

	auto regionSize = Region_.GetSize();
	for(size_t y = 0; y < regionSize.y; ++y)
		for(size_t x = 0; x < regionSize.x; ++x)
		{
			
		}

	Mesh_ = new graphics::RStaticMesh();
	graphics::RenderResourceManager::CreateStaticMesh(Mesh_, indices, vertexData);
}
