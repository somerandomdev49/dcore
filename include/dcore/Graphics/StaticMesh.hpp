#pragma once
#include <dcore/Resource/ResourceManager.hpp>
#include <fwdraw.hpp>
#include <vector>

namespace dcore::graphics
{
	class StaticMesh
	{
	public:
	private:
		resource::Resource<fwdraw::Mesh> Mesh_;
		std::vector<resource::Resource<fwdraw::Texture>> Textures_;
	};
}
