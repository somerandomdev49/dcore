#pragma once
#include <dcore/Renderer/RStaticMesh.hpp>
#include <dcore/Renderer/RTexture.hpp>
#include <dcore/Renderer/Config.hpp>

#include <vector>

namespace dcore::graphics
{
	class RModel
	{
		std::vector<RStaticMesh*> Meshes_;
		std::vector<RTexture*> Textures_;
	public:
		
	};
}
