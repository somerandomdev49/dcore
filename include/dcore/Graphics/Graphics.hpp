#pragma once
#include <dcore/Uni.hpp>
#include <glm/glm.hpp>
#include <dcore/Graphics/Shader.hpp>
#include <dcore/Graphics/StaticMesh.hpp>
#include <dcore/Renderer/Renderer.hpp>
// #include <dcore/Platform/Platform.hpp>
#include <dcore/Graphics/Camera.hpp>
#include <dcore/Resource/ResourceManager.hpp>

namespace dcore::platform
{
	class Context;
}

namespace dcore::graphics
{
	/** Provides helper functions for rendering with dcore::graphics::Renderer */
	class RendererInterface
	{
	public:
		Renderer *GetRenderer() const;
		// void RenderCharacter(Character *character);
		void RenderStaticMesh(const StaticMesh DCORE_REF *staticMesh);
		void RenderDebugLine(const glm::vec3 &from, const glm::vec3 &to, float width = 1.f);

		Camera *GetCamera() const;

	private:
		friend class platform::Context;
		void Initialize(resource::ResourceManager DCORE_REF *rm, Renderer DCORE_REF *rend);
		void DeInitialize();

		Camera DCORE_OWN *Camera_;
		CommonShader DCORE_OWN *ObjectShader_;
		Renderer DCORE_REF *Renderer_;
	};
} // namespace dcore::graphics
