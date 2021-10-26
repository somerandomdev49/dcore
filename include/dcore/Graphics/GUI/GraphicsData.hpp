#pragma once
#include <dcore/Renderer/RTexture.hpp>
#include <glm/glm.hpp>

namespace dcore::graphics::gui
{
	struct Quad
	{
		glm::vec2 Position;
		glm::vec2 Scale;
		float Rotation;
		RTexture *Texture;
		glm::vec4 Color;
	};
} // namespace dcore::graphics::gui
