#pragma once
#include "dcore/Renderer/RSkyBox.hpp"
#include "dcore/Renderer/RStaticMesh.hpp"
#include <dcore/Uni.hpp>
#include <glm/glm.hpp>
#include <dcore/Graphics/Shader.hpp>
#include <dcore/Graphics/StaticMesh.hpp>
#include <dcore/Renderer/Renderer.hpp>
#include <dcore/Graphics/Camera.hpp>
#include <dcore/Resource/ResourceManager.hpp>
#include <dcore/Misc/Terrain/Chunk.hpp>
#include <dcore/Renderer/RModel.hpp>

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
		Renderer *GetRenderer() const { return Renderer_; }
		// void RenderCharacter(Character *character);
		void RenderStaticMesh(const StaticMesh DCORE_REF *staticMesh, const glm::mat4 &transform);
		void RenderModel(const Model DCORE_REF *model, const glm::mat4 &transform);
		void RenderChunk(const terrain::Chunk *chunk);
		void RenderDebugLine(const glm::vec3 &from, const glm::vec3 &to, float width = 1.f);
		void RenderSkyBox(const RSkyBox DCORE_REF *skybox);
		void RenderSkyBox(const RSkyBox DCORE_REF *skybox1, const RSkyBox DCORE_REF *skybox2, float t);

		Camera *GetCamera() const { return Camera_; }

		const glm::vec3 &GetFogColor() { return FogColor_; }
		void SetFogColor(glm::vec3 newColor);

	private:
		friend class platform::Context;
		void Initialize(resource::ResourceManager DCORE_REF *rm, Renderer DCORE_REF *rend = nullptr);
		void DeInitialize();

		Camera DCORE_OWN *Camera_;
		CommonShader DCORE_OWN *ObjectShader_;
		TerrainShader DCORE_OWN *TerrainShader_;
		CommonShader DCORE_OWN *SkyBoxShader_, DCORE_OWN *SkyBoxTransShader_;
		RStaticMesh *SkyBoxMesh_;
		glm::vec3 FogColor_;
		Renderer DCORE_REF *Renderer_;
	};
} // namespace dcore::graphics
