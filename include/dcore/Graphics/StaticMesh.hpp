#pragma once
#include <vector>
#include <dcore/Resource/ResourceManager.hpp>

namespace dcore::graphics
{
	class StaticMesh
	{
	public:
	private:
		Resource<fwdraw::Mesh> Mesh_;
		std::vector<Resource<fwdraw::Texture>> Textures_;
	};
}
