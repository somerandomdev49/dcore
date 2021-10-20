#pragma once
#include <dcore/Uni.hpp>
#include <glm/glm.hpp>
#include <dcore/Graphics/Shader.hpp>
#include <dcore/Graphics/StaticMesh.hpp>
#include <dcore/Renderer/Renderer.hpp>
#include <dcore/Graphics/Camera.hpp>
#include <dcore/Resource/ResourceManager.hpp>
#include <dcore/Misc/Terrain/Chunk.hpp>

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
		void RenderChunk(const terrain::Chunk *chunk);
		void RenderDebugLine(const glm::vec3 &from, const glm::vec3 &to, float width = 1.f);

		Camera *GetCamera() const;

	private:
		friend class platform::Context;
		void Initialize(resource::ResourceManager DCORE_REF *rm, Renderer DCORE_REF *rend = nullptr);
		void DeInitialize();

		Camera DCORE_OWN *Camera_;
		CommonShader DCORE_OWN *ObjectShader_;
		TerrainShader DCORE_OWN *TerrainShader_;
		Renderer DCORE_REF *Renderer_;
	};
} // namespace dcore::graphics
