#pragma once
#include <dcore/Uni.hpp>
#include <glm/glm.hpp>
#include <dcore/Graphics/Character.hpp>
#include <dcore/Renderer/Renderer.hpp>
#include <dcore/Launch.hpp>
#include <dcore/Resource/ResourceManager.hpp>

namespace dcore::graphics
{
	/** Provides helper functions for rendering with dcore::graphics::Renderer */
	class RendererInterface
	{
		Renderer *GetRenderer();
		// void RenderCharacter(Character *character);
		void RenderStaticMesh(StaticMesh *staticMesh);
		void RenderDebugLine(const glm::vec3 &from, const glm::vec3 &to, float width = 1.f);
	private:
		friend class launch::Launch;
		void Initialize();
		void DeInitialize();

		Shader OWN *ObjectShader;
		Renderer *Renderer_;
	};
}
