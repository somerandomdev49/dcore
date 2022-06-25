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
	Chunk::Chunk(const HeightmapRegion &region, const glm::ivec2 &localPosition)
	    : Region_(region), LocalPosition_(localPosition)
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

	namespace {
		glm::uvec2 GenerateVertices(HeightmapRegion &region, glm::uvec2 vertexCounts, std::vector<byte> &vertexData)
		{
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
			
			/** Total vertex count */
			dstd::UInt32 vertexCount = 0;

	
			auto regionMin = (glm::ivec2)region.GetMin();
			auto regionMax = (glm::ivec2)region.GetMax();
			auto regionSize = (glm::ivec2)region.GetSize();
			auto *heightmap = region.GetSource();
			
			for(int y = 0; y < (int)vertexCounts.y; ++y)
				for(int x = 0; x < (int)vertexCounts.x; ++x)
				{
					float h = region.Get(glm::uvec2(x, y)) * Terrain::GetCHeight();

					// Position
					pushVec3(glm::vec3(
						(x * Terrain::GetCUnitsPerPixel()
							- (dstd::USize)(Terrain::GetCChunkSize() / 2)),
						h,
						(y * Terrain::GetCUnitsPerPixel()
							- (dstd::USize)(Terrain::GetCChunkSize() / 2))
					));
					
					// Normal
					float N = (y == 0 && regionMin.y == 0)
						? 0 : heightmap->Get(regionMin + glm::ivec2 { x, y - 1 });
					
					float S = (y == regionSize.y - 1 && regionMax.y == (int)heightmap->GetSize().y)
						? 0 : heightmap->Get(regionMin + glm::ivec2 { x, y + 1 });
					
					float W = (x == 0 && regionMin.x == 0)
						? 0 : heightmap->Get(regionMin + glm::ivec2 { x - 1, y });
					
					float E = (x == regionSize.x - 1 && regionMax.x == (int)heightmap->GetSize().x)
						? 0 : heightmap->Get(regionMin + glm::ivec2 { x + 1, y });

					glm::vec3 normal = { 0, 1, 0 };
					normal.z += N;
					normal.z -= S;
					normal.x += W;
					normal.x -= E;
					pushVec3(glm::normalize(normal));

					// Texture Coordinate
					pushVec2(glm::vec2(x / (float)vertexCounts.x, y / (float)vertexCounts.y));

					++vertexCount;
				}
			
			return vertexCounts;
		}

		void GenerateIndices(HeightmapRegion &region, glm::uvec2 vertexCount, std::vector<uint32_t> &indices)
		{
			// generating indices
			indices.reserve((vertexCount.x - 1) * (vertexCount.y - 1) * 2 * 3); // 2 tris per pixel, 3 verts per tri

			for(dstd::USize y = 0, i = 0; y < vertexCount.y - 1; ++y, ++i)
			{
				for(dstd::USize x = 0; x < vertexCount.x - 1; ++x, ++i)
				{
					// for each pixel (x, y) or for each "top-left" vertex of a triangle
					indices.push_back(i);
					indices.push_back(i + 1);
					indices.push_back(i + vertexCount.x);
					indices.push_back(i + 1);
					indices.push_back(i + vertexCount.x + 1);
					indices.push_back(i + vertexCount.x);

				}
			}
		}
	}

	void Chunk::GenerateMesh_()
	{
		std::vector<uint32_t> indices;
		std::vector<byte> vertexData;
		auto vertexCount = Region_.GetSize() + glm::uvec2(1, 1);

		GenerateVertices(Region_, vertexCount, vertexData);
		GenerateIndices(Region_, vertexCount, indices);

		Mesh_ = new graphics::RStaticMesh();
		graphics::RenderResourceManager::CreateStaticMesh(Mesh_, indices, vertexData);
	}

	const resource::Resource<graphics::RTexture> &Chunk::GetBlendMap() const { return BlendMap_; }
	const resource::Resource<graphics::RTexture> *Chunk::GetTextures() const { return &Textures_[0]; }
	void Chunk::SetBlendMap(const resource::Resource<graphics::RTexture> &newBlendMap)
	{
		BlendMap_ = newBlendMap;
	}

	void Chunk::SetTexture(int index, const resource::Resource<graphics::RTexture> &newTexture)
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
