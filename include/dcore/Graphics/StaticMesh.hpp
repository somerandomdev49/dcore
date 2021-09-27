#pragma once
#include <dcore/Resource/ResourceManager.hpp>
#include <dcore/Graphics/Renderable.hpp>
#include <fwdraw.hpp>
#include <vector>

namespace dcore::graphics
{
	class StaticMesh : public Renderable
	{
	public:
		StaticMesh(const resource::Resource<fwdraw::Mesh> &mesh, const resource::Resource<fwdraw::Texture> &texture);

		const resource::Resource<fwdraw::Mesh> &GetMesh() const;
		const resource::Resource<fwdraw::Texture> &GetTexture() const;
	private:
		resource::Resource<fwdraw::Mesh> Mesh_;
		resource::Resource<fwdraw::Texture> Texture_;
	};
}
