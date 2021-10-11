#include <dcore/Game/Terrain/Heightmap.hpp>
#include <dcore/Renderer/RStaticMesh.hpp>

namespace dg::terrain
{
	class Chunk
	{
	public:
		static void Register();

		void Initialize(const HeightmapRegion &region);
		const HeightmapRegion &GetRegion() const;
		const glm::vec2 &GetSize() const;
		const glm::vec2 &GetGlobalPosition() const;
		const glm::ivec2 &GetLocalPosition() const;
		void DeInitialize();

	private:
		void Load(const std::string &id, const std::string &location, dcore::resource::ResourceManager *res);

		HeightmapRegion Region_;
		dcore::resource::Resource<dcore::graphics::RStaticMesh> BlendMap_;
		dcore::graphics::RStaticMesh DCORE_OWN *Mesh_;
	};
} // namespace dg::terrain
