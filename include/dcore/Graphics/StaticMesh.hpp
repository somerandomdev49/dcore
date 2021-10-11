#pragma once
#include <dcore/Renderer/RTexture.hpp>
#include <dcore/Renderer/RStaticMesh.hpp>
#include <dcore/Resource/ResourceManager.hpp>
#include <dcore/Graphics/Renderable.hpp>
// #include <fwdraw.hpp>
#include <vector>

namespace dcore::graphics {
	class StaticMesh : public Renderable {
	public:
		StaticMesh(const resource::Resource<RStaticMesh> &mesh,
		           const resource::Resource<RTexture> &texture);

		const resource::Resource<RStaticMesh> &GetMesh() const;
		const resource::Resource<RTexture> &GetTexture() const;

	private:
		resource::Resource<RStaticMesh> Mesh_;
		resource::Resource<RTexture> Texture_;
	};
} // namespace dcore::graphics
