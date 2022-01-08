#include <dcore/Misc/Terrain/Chunk.hpp>
#include <dcore/Misc/Terrain/Terrain.hpp>
#include <dcore/Renderer/Renderer.hpp>
#include <dcore/Core/FrameLog.hpp>
#include <dcore/Core/Log.hpp>
#include <dcore/Uni.hpp>

inline std::ostream &operator<<(std::ostream &os, const glm::ivec2 &v)
{
	os << "(" << v.x << ", " << v.y << ")";
	return os;
}

namespace dcore::terrain
{
	Chunk::Chunk(HeightmapRegion &&region, const glm::ivec2 &localPosition)
	    : Region_(std::move(region)), LocalPosition_(localPosition)
	{
	}

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
		std::vector<byte> vertexData;

		const auto pushFloat = [&](float f)
		{
			byte *bytes = reinterpret_cast<byte *>(&f);
			vertexData.push_back(bytes[0]);
			vertexData.push_back(bytes[1]);
			vertexData.push_back(bytes[2]);
			vertexData.push_back(bytes[3]);
		};

		const auto pushVec2 = [&](const glm::vec2 &v)
		{
			pushFloat(v.x);
			pushFloat(v.y);
		};

		const auto pushVec3 = [&](const glm::vec3 &v)
		{
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
		dstd::UInt32 vertexCount = 0;
		auto regionSize          = Region_.GetSize() + glm::uvec2(1, 1);
		// DCORE_LOG_INFO << "Region Size: " << regionSize;
		for(dstd::UInt32 y = 0; y < regionSize.y; ++y)
			for(dstd::UInt32 x = 0; x < regionSize.x; ++x)
			{
				float h = Region_.Get(glm::uvec2(x, y)) * Terrain::GetCHeight();
				pushVec3(glm::vec3((x * Terrain::GetCUnitsPerPixel() - Terrain::GetCChunkSize() / 2), h,
				                   (y * Terrain::GetCUnitsPerPixel() - Terrain::GetCChunkSize() / 2))); // position
				pushVec3(glm::vec3(0, 0, 0));                                                           // normal
				pushVec2(glm::vec2(x / (float)regionSize.x, y / (float)regionSize.y));                  // texcoord
				++vertexCount;
			}

		// generating indices
		// DCORE_LOG_INFO << "regionSize " << regionSize << ", Index Count: " << regionSize.x * regionSize.y * 6;
		indices.reserve(regionSize.x * (regionSize.y - 1) * 6);
		// using Vertex = graphics::RenderResourceManage;
		struct Vertex
		{
			glm::vec3 pos;
			glm::vec3 norm;
			glm::vec2 tex;
		};
		const auto setNormals = [&](int a, int b, int c)
		{
			Vertex *vertices = reinterpret_cast<Vertex *>(&vertexData[0]);
			Vertex *v1 = &vertices[a], *v2 = &vertices[b], *v3 = &vertices[c];
			auto u = v1->pos - v2->pos, v = v1->pos - v3->pos;
			auto n = glm::normalize(glm::cross(u, v));
			v1->norm += n;
			v2->norm += n;
			v3->norm += n;
		};

		dstd::UInt32 vert = 0;
		for(dstd::UInt32 y = 0; y < regionSize.y - 1; ++y)
		{
			for(dstd::UInt32 x = 0; x < regionSize.x; ++x)
			{
				if(x == regionSize.x - 1)
				{
					++vert;
					continue;
				}
				indices.push_back(vert + 0);
				indices.push_back(vert + 1);
				indices.push_back(vert + regionSize.x);
				setNormals(vert + 0, vert + 1, vert + regionSize.x);

				indices.push_back(vert + 1);
				indices.push_back(vert + regionSize.x + 1);
				indices.push_back(vert + regionSize.x);
				setNormals(vert + 1, vert + regionSize.x + 1, vert + regionSize.x);

				++vert;
			}
		}
		indices.shrink_to_fit();

		for(uint32_t index : indices)
		{
			Vertex *vertices     = reinterpret_cast<Vertex *>(&vertexData[0]);
			vertices[index].norm = glm::normalize(vertices[index].norm);
		}

		// DCORE_LOG_INFO << "Done generating chunk. Region Size: " << regionSize << ", Vertex
		// Count: " << vertexCount
		// << ", Index Count: " <<
		indices.size();

		// size_t vertexSize = sizeof(float) * (3 + 3 + 2);
		// DCORE_LOG_INFO << "Index Count: " << indices.size() << ", Vertex Count: " <<
		// (float)vertexData.size() / (float)vertexSize;

		Mesh_ = new graphics::RStaticMesh();
		graphics::RenderResourceManager::CreateStaticMesh(Mesh_, indices, vertexData);
	}

	const dcore::resource::Resource<dcore::graphics::RTexture> &Chunk::GetBlendMap() const { return BlendMap_; }
	const dcore::resource::Resource<dcore::graphics::RTexture> *Chunk::GetTextures() const { return &Textures_[0]; }
	void Chunk::SetBlendMap(const dcore::resource::Resource<dcore::graphics::RTexture> &newBlendMap)
	{
		BlendMap_ = newBlendMap;
	}

	void Chunk::SetTexture(int index, const dcore::resource::Resource<dcore::graphics::RTexture> &newTexture)
	{
		Textures_[index] = newTexture;
	}

	float Chunk::GetHeightAtGlobal(const glm::vec2 &v) const { return GetHeightAtLocal(v - GetGlobalPosition()); }

	// TODO: Generate mesh which is smaller than the chunk region by 1 on each axis.
	float Chunk::GetHeightAtLocal(const glm::vec2 &v) const
	{
		glm::uvec2 grid = v / (float)Terrain::GetCUnitsPerPixel();
		FrameLog::SLogF("ch: grid pos: %d, %d", grid.x, grid.y);

		glm::uvec2 d(1, 1);
		if(grid.x >= Region_.GetSize().x - 1) d.x = 0;
		if(grid.y >= Region_.GetSize().y - 1) d.y = 0;

		float p00 = Region_.Get(glm::ivec2(grid.x + 0, grid.y + 0)),
		      p01 = Region_.Get(glm::ivec2(grid.x + 0, grid.y + d.y)),
		      p10 = Region_.Get(glm::ivec2(grid.x + d.x, grid.y + 0)),
		      p11 = Region_.Get(glm::ivec2(grid.x + d.x, grid.y + d.y));

		FrameLog::SLogF("00 - %.2f, 01 - %.2f, 10 - %.2f, 11 - %.2f", p00, p01, p10, p11);

		glm::vec2 pos = glm::vec2(std::fmod(v.x, Terrain::GetCChunkSize()), std::fmod(v.x, Terrain::GetCChunkSize())) /
		                (float)Terrain::GetCChunkSize();

		return (p00 * (1 - pos.x) * (1 - pos.y) + p10 * (0 + pos.x) * (1 - pos.y) + p01 * (1 - pos.x) * (0 + pos.y) +
		        p11 * (0 + pos.x) * (0 + pos.y)) *
		       Terrain::GetCHeight();
	}

	glm::vec2 Chunk::GetGlobalPosition() const
	{
		return glm::vec2(LocalPosition_) * Terrain::GetCUnitsPerPixel() * (float)Terrain::GetCChunkSize();
	}
	const glm::ivec2 &Chunk::GetLocalPosition() const { return LocalPosition_; }
	dcore::graphics::RStaticMesh *Chunk::GetMesh() const { return Mesh_; }
} // namespace dcore::terrain
