#pragma once
#include <cinttypes>
#include <unordered_map>

namespace dcore::resource
{
	enum ResourceType : std::uint16_t
	{
		STATIC_MESH,
		SKELETAL_MESH,
		TEXTURE_2D,
		TEXTURE_3D,
		TEXTURE_CUBEMAP,
		SHADER
	};

	/** Manages all of the resources of the game. */
	class ResourceManager
	{
		std::unordered_map<ResourceType> Resources_;
	};
}
