#include <dcore/Misc/Terrain/Heightmap.hpp>
#include <dcore/Util/LoaderUtil.hpp>
#include <dcore/Uni.hpp>
#include <cstring>
#include <cstdlib>

namespace dcore::terrain
{

	void Heightmap::Heightmap_Constructor(const std::string &path, void *placement)
	{
		auto h = new(placement) Heightmap();

		util::ImageData data;
		util::LoaderUtil::LoadImage(data, path);

		h->Size_           = data.size;
		h->Data_           = new byte[h->Size_.x * h->Size_.y * data.channels];
		h->ComponentCount_ = data.channels;
		std::memcpy(h->Data_, data.data, h->Size_.x * h->Size_.y * data.channels);
		std::free(data.data);
		printf("Component Count: %d, size: %d, %d\n", h->ComponentCount_, data.size.x, data.size.y);
	}

	void Heightmap::Heightmap_DeConstructor(void *placement)
	{
		Heightmap *hm = (Heightmap *)placement;
		delete hm->Data_;
	}

	const glm::ivec2 &Heightmap::GetSize() const { return Size_; }
	float Heightmap::Get(const glm::ivec2 &pos) const
	{
		return (Data_[(pos.x + pos.y * Size_.x) * ComponentCount_] & 0xff) / 255.0f;
	}

	HeightmapRegion::HeightmapRegion(Heightmap *source, const glm::ivec2 &min, const glm::ivec2 &max)
	    : Min_(min), Max_(max), Source_(source)
	{
	}
	glm::ivec2 HeightmapRegion::GetSize() const { return GetMax() - GetMin(); }
	const glm::ivec2 &HeightmapRegion::GetMax() const { return Max_; }
	const glm::ivec2 &HeightmapRegion::GetMin() const { return Min_; }

	float HeightmapRegion::Get(const glm::ivec2 &pos) const { return Source_->Get(Min_ + pos); }
} // namespace dcore::terrain
