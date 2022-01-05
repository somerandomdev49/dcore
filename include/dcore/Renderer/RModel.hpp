#pragma once
#include <dcore/Renderer/RStaticMesh.hpp>
#include <dcore/Renderer/RTexture.hpp>
#include <dcore/Renderer/Config.hpp>

#include <vector>

namespace dcore::graphics
{
	/** Used by Model */
	struct TextureSlot
	{
		std::string Name;
		RTexture *Texture;
	};

	/** Used by Model */
	struct ModelMesh
	{
		RStaticMesh *Mesh;
		dstd::USize TextureIndex;
	};

	class Model
	{
		std::vector<ModelMesh> Meshes_;
		std::vector<TextureSlot> TextureSlots_;
		friend class Renderer;
		friend class RenderResourceManager;

	public:
	};
} // namespace dcore::graphics
