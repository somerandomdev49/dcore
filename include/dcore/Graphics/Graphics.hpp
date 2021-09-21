#pragma once
#include <glm/glm.hpp>
#include <dcore/Graphics/Character.hpp>
#include <dcore/Renderer/Renderer.hpp>

namespace dcore::graphics
{
	class Graphics
	{
	public:
		void Initialize();
		void DeInitialize();
	};

	class RendererInterface
	{
		Renderer *GetRenderer();
		void RenderCharacter(Character *character);
		void RenderStaticMesh(StaticMesh *staticMesh);
		void RenderDebugLine(const glm::vec3 &from, const glm::vec3 &to, float width = 1.f);
	private:
		Renderer *Renderer_;
	};
}
