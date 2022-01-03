#pragma once
#include <dcore/Renderer/RStaticMesh.hpp>
#include <dcore/Renderer/RTexture.hpp>
#include <dcore/Renderer/Config.hpp>

#include <vector>

namespace dcore::graphics
{
	struct TextureSlot
	{
		std::string Name;
		RTexture *Texture;
	};

	class Model
	{
		std::vector<RStaticMesh *> Meshes_;
		std::vector<TextureSlot> TextureSlots_;

	public:
	};
} // namespace dcore::graphics
