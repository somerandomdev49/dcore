#pragma once
#include <dcore/Renderer/RTexture.hpp>
#include <glm/glm.hpp>

namespace dcore::graphics::gui
{
	struct Quad final
	{
		glm::vec2 Position;
		glm::vec2 Scale;
		float Rotation;
		RTexture *Texture;
		glm::vec4 Color;

		bool OverlapPoint(const glm::vec2 &pointer);
		bool OverlapQuad(const Quad &other);
	};
} // namespace dcore::graphics::gui
