#include <dcore/Misc/Terrain/Heightmap.hpp>
#include <dcore/Util/LoaderUtil.hpp>
#include <cstring>
#include <cstdlib>

using namespace dcore::terrain;

void Heightmap::Heightmap_Constructor(const std::string &path, void *placement)
{
	auto h = new(placement) Heightmap();

	util::ImageData data;
	util::LoaderUtil::LoadImage(data, path);

	h->Size_ = data.size;
	h->Data_ = new uint32_t[h->Size_.x * h->Size_.y];
	std::memcpy(h->Data_, data.data, h->Size_.x * h->Size_.y);
	std::free(data.data);
}

void Heightmap::Heightmap_DeConstructor(void *placement)
{
	Heightmap *hm = (Heightmap *)placement;
	delete hm->Data_;
}

const glm::ivec2 &Heightmap::GetSize() const { return Size_; }

HeightmapRegion::HeightmapRegion(Heightmap *source, const glm::ivec2 &min, const glm::ivec2 &max) : Min_(min), Max_(max), Source_(source) {}
glm::ivec2 HeightmapRegion::GetSize() const { return GetMax() - GetMin(); }
const glm::ivec2 &HeightmapRegion::GetMax() const { return Max_; }
const glm::ivec2 &HeightmapRegion::GetMin() const { return Min_; }
