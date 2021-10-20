#pragma once
#include <dcore/Renderer/RFastVertexBuffer.hpp>
#include <dcore/Graphics/GUI/GraphicsData.hpp>
#include <dcore/Resource/ResourceManager.hpp>
#include <dcore/Renderer/RStaticMesh.hpp>
#include <dcore/Graphics/GUI/Widget.hpp>
#include <dcore/Renderer/RTexture.hpp>
#include <dcore/Renderer/RShader.hpp>
#include <dcore/Launch.hpp>

namespace dcore::graphics::gui
{
	class GuiShader
	{
	public:
		GuiShader(const resource::Resource<RShader> &sh);
		RShader *Get() const;

		void SetTexture(int unit);
		void SetColor(const glm::vec4 &color);
		void SetTransform(const glm::mat3 &m);

	private:
		resource::Resource<RShader> Shader_;
		RUniform UTransform_;
		RUniform UTex_;
		RUniform UColor_;
	};

	class GuiGraphics
	{
	public:
		static GuiGraphics *Instance();
		void Initialize(resource::ResourceManager DCORE_REF *rm, Renderer DCORE_REF *rend = nullptr);
		void DeInitialize();
		void RenderQuad(const Quad &quad);

	private:
		friend class launch::Launch;
		static void SetInstance(GuiGraphics *newInstance);
		GuiShader *GuiShader_;
		RFastVertexBuffer *Quad_;
		Renderer *Rend_;
	};
} // namespace dcore::graphics::gui
