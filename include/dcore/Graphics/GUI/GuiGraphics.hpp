#pragma once
#include <dcore/Renderer/RFastVertexBuffer.hpp>
#include <dcore/Graphics/GUI/GraphicsData.hpp>
#include <dcore/Graphics/GUI/Font.hpp>
#include <dcore/Resource/ResourceManager.hpp>
#include <dcore/Renderer/RStaticMesh.hpp>
#include <dcore/Renderer/RTexture.hpp>
#include <dcore/Renderer/RShader.hpp>
#include <dcore/Launch.hpp>

namespace dcore::graphics::gui
{
	class CommonGuiShader
	{
	public:
		virtual ~CommonGuiShader();
		virtual void     SetTexture(int unit)              = 0;
		virtual void     SetColor(const glm::vec4 &color)  = 0;
		virtual void     SetTransform(const glm::mat4 &m)  = 0;
		virtual void     SetProjection(const glm::mat4 &m) = 0;
		virtual RShader *Get() const                       = 0;
	};

	class GuiShader : public CommonGuiShader
	{
	public:
		GuiShader(const resource::Resource<RShader> &sh);
		~GuiShader();

		void     SetTexture(int unit) override;
		void     SetColor(const glm::vec4 &color) override;
		void     SetTransform(const glm::mat4 &m) override;
		void     SetProjection(const glm::mat4 &m) override;
		RShader *Get() const override;

	private:
		resource::Resource<RShader> Shader_;
		RUniform                    UTransform_;
		RUniform                    UProjection_;
		RUniform                    UTex_;
		RUniform                    UColor_;
	};

	class FontShader : public CommonGuiShader
	{
	public:
		FontShader(const resource::Resource<RShader> &sh);
		~FontShader();

		void     SetTexture(int unit) override;
		void     SetColor(const glm::vec4 &color) override;
		void     SetTransform(const glm::mat4 &m) override;
		void     SetProjection(const glm::mat4 &m) override;
		void     SetTexCoords(const glm::vec4 &begin, const glm::vec4 &end);
		RShader *Get() const override;

	private:
		resource::Resource<RShader> Shader_;
		RUniform                    UTransform_;
		RUniform                    UProjection_;
		RUniform                    UTex_;
		RUniform                    UColor_;
		RUniform                    UTexCoords_[4];
	};

	class GuiGraphics
	{
	public:
		static GuiGraphics *Instance();
		void                Initialize(resource::ResourceManager DCORE_REF *rm, Renderer DCORE_REF *rend = nullptr);
		void                DeInitialize();
		void                RenderQuad(const Quad &quad);
		void RenderText(Font *font, const char *text, const glm::vec2 &origin, int size = -1, float scale = -1);

	private:
		friend class launch::Launch;
		static void        SetInstance(GuiGraphics *newInstance);
		void               RenderQuad_(const Quad &quad, CommonGuiShader *shader, bool bind = true);
		GuiShader         *GuiShader_;
		FontShader        *FontShader_;
		RFastVertexBuffer *Quad_;
		Renderer          *Rend_;
	};
} // namespace dcore::graphics::gui
