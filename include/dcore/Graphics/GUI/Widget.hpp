#pragma once
#include <dcore/Graphics/GUI/GraphicsData.hpp>
#include <dcore/Renderer/RTexture.hpp>
#include <dcore/Uni.hpp>
#include <glm/glm.hpp>
#include <vector>

namespace dcore::graphics::gui
{
	class Widget
	{
	public:
		const glm::vec2 &GetPosition() const;
		void SetPosition(const glm::vec2 &newPosition);

		float GetRotation() const;
		void SetRotation(float newRotation);

		const glm::vec2 &GetSize() const;
		void SetSize(const glm::vec2 &newSize);

	private:
		Quad Quad_;
		RTexture *Texture_;
		DCORE_REF Widget *Parent_;
		std::vector<DCORE_OWN Widget *> Children_;
	};
} // namespace dcore::graphics::gui
