#include <dcore/Graphics/GUI/GuiGraphics.hpp>
#include <dcore/Graphics/GUI/Font.hpp>
#include <dcore/Renderer/Renderer.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <dcore/Core/Preferences.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace dcore::graphics::gui
{
	CommonGuiShader::~CommonGuiShader() {}

	/**************************** GuiShader ****************************/

	GuiShader::~GuiShader() {}
	GuiShader::GuiShader(const dcore::resource::Resource<dcore::graphics::RShader> &shader)
	{
		Shader_      = shader;
		UTransform_  = Renderer::Instance()->GetUniform(Shader_.Get(), "u_Transform");
		UProjection_ = Renderer::Instance()->GetUniform(Shader_.Get(), "u_Projection");
		UTex_        = Renderer::Instance()->GetUniform(Shader_.Get(), "u_Tex");
		UColor_      = Renderer::Instance()->GetUniform(Shader_.Get(), "u_Color");
		SetTexture(0);
	}

	void GuiShader::SetColor(const glm::vec4 &v) { Renderer::Instance()->SetUniform(UColor_, v); }
	void GuiShader::SetTransform(const glm::mat4 &v) { Renderer::Instance()->SetUniform(UTransform_, v); }
	void GuiShader::SetProjection(const glm::mat4 &v) { Renderer::Instance()->SetUniform(UProjection_, v); }
	void GuiShader::SetTexture(int unit) { Renderer::Instance()->SetUniform(UColor_, unit); }
	dcore::graphics::RShader *GuiShader::Get() const { return Shader_.Get(); }

	/**************************** FontShader ****************************/

	FontShader::~FontShader() {}
	FontShader::FontShader(const dcore::resource::Resource<dcore::graphics::RShader> &shader)
	{
		static const char *uniformNames[4] = {"u_TexCoords[0]", "u_TexCoords[1]", "u_TexCoords[2]", "u_TexCoords[3]"};

		Shader_      = shader;
		UTransform_  = Renderer::Instance()->GetUniform(Shader_.Get(), "u_Transform");
		UProjection_ = Renderer::Instance()->GetUniform(Shader_.Get(), "u_Projection");
		UTex_        = Renderer::Instance()->GetUniform(Shader_.Get(), "u_Tex");
		UColor_      = Renderer::Instance()->GetUniform(Shader_.Get(), "u_Color");
		for(int i = 0; i < 4; ++i)
		{
			UTexCoords_[i] = Renderer::Instance()->GetUniform(Shader_.Get(), uniformNames[i]);
		}
		SetTexture(0);
	}

	void FontShader::SetColor(const glm::vec4 &v) { Renderer::Instance()->SetUniform(UColor_, v); }
	void FontShader::SetTransform(const glm::mat4 &v) { Renderer::Instance()->SetUniform(UTransform_, v); }
	void FontShader::SetProjection(const glm::mat4 &v) { Renderer::Instance()->SetUniform(UProjection_, v); }
	void FontShader::SetTexture(int unit) { Renderer::Instance()->SetUniform(UColor_, unit); }
	void FontShader::SetTexCoords(const glm::vec4 &s, const glm::vec4 &e)
	{
		Renderer::Instance()->SetUniform(UTexCoords_[0], glm::vec2(s.x, s.y));
		Renderer::Instance()->SetUniform(UTexCoords_[1], glm::vec2(s.z, s.w));
		Renderer::Instance()->SetUniform(UTexCoords_[2], glm::vec2(e.x, e.y));
		Renderer::Instance()->SetUniform(UTexCoords_[3], glm::vec2(e.z, e.w));
	}
	dcore::graphics::RShader *FontShader::Get() const { return Shader_.Get(); }

	/**************************** Quad ****************************/

	// TODO(!): Rotation
	bool Quad::OverlapPoint(const glm::vec2 &pointer)
	{
		return pointer.x >= Position.x && pointer.x <= Position.x + Scale.x && pointer.y >= Position.y &&
		       pointer.y <= Position.y + Scale.y;
	}

	bool Quad::OverlapQuad(const Quad &other) { return false; }

	/**************************** GuiGraphics ****************************/

	void GuiGraphics::Initialize(resource::ResourceManager DCORE_REF *rm, Renderer DCORE_REF *rend)
	{
		if(rend == nullptr) rend = Renderer::Instance();
		Rend_ = rend;

		GuiShader_    = new GuiShader(rm->Get<RShader>("DCore.Shader.GuiShader"));
		FontShader_   = new FontShader(rm->Get<RShader>("DCore.Shader.FontShader"));
		Quad_         = new RFastVertexBuffer();
		SolidTexture_ = new RTexture();

		byte data[] = {0xff, 0xff, 0xff, 0xff};
		RenderResourceManager::CreateTexture(SolidTexture_, data, glm::ivec2(1, 1),
		                                     RenderResourceManager::TextureFormat::Rgba,
		                                     RenderResourceManager::TextureScaling::Nearest);

		RenderResourceManager::CreateFastVertexBuffer(Quad_, 4);
	}

	void GuiGraphics::DeInitialize()
	{
		delete GuiShader_;
		delete FontShader_;
		RenderResourceManager::DeleteFastVertexBuffer(Quad_);
		delete Quad_;
		RenderResourceManager::DeleteTexture(SolidTexture_);
		delete SolidTexture_;
	}

	void GuiGraphics::RenderQuad_(const Quad &quad, CommonGuiShader *shader, bool bind)
	{
		if(!shader) return;
		if(bind)
		{
			Rend_->UseShader(shader->Get());
			if(quad.Texture) Rend_->UseTexture(0, quad.Texture);
			else Rend_->UseTexture(0, SolidTexture_);
		}

		auto tr = glm::mat4(1.0f);
		tr      = glm::translate(tr, glm::vec3(quad.Position, 0.0f));
		tr      = glm::rotate(tr, quad.Rotation, glm::vec3(0, 0, 1));
		tr      = glm::scale(tr, glm::vec3(quad.Scale, 1.0f));

		auto res = Preferences::Instance()->GetDisplaySettings().Resolution;
		auto pr = glm::ortho(0.0f, (float)res.x, (float)res.y, 0.0f, -1.0f, 1.0f);

		shader->SetTransform(tr);
		shader->SetProjection(pr);
		shader->SetColor(quad.Color);
		// Texture is 0 by default.

		Rend_->Render(Quad_);
	}

	// TODO: inline?
	void GuiGraphics::RenderQuad(const Quad &quad) { RenderQuad_(quad, GuiShader_); }

	void GuiGraphics::RenderText(Font *font, const char *text, const glm::vec2 &origin, int size, float scale)
	{
		if(!text) return;
		float pixelScale = 1.0f;
		if(size > 0) pixelScale *= float(size) / float(font->PixelHeight_);
		if(scale > 0) pixelScale *= scale;
		// glm::vec2 alignedOrigin = origin + glm::vec2(0.0f, font->GetAscent() * pixelScale + size / 2);

		// Rend_->DisableDepthCheck();

		// Bind once!
		Rend_->UseShader(FontShader_->Get());

		// char c = text[0];
		glm::vec2 cursor = origin;

		auto textLength = std::strlen(text);
		for(size_t idx = 0; idx < textLength; ++idx)
		{
			if(text[idx] >= 32)
			{
				auto &cp      = font->CodePointTable_[text[idx] - ' '];
				glm::vec2 pos = glm::vec2(cursor.x + cp.Bearing.x * pixelScale, cursor.y - cp.Bearing.y * pixelScale);

				if(idx < textLength - 1) cursor.x += font->GetKerning(text[idx], text[idx - 1]);
				cursor.x += (cp.AdvanceWidth >> 6) * pixelScale;

				Quad q;
				q.Color    = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
				q.Position = pos;
				q.Scale    = glm::vec2(cp.AtlasSize) * pixelScale;
				q.Rotation = 0.0f;
				q.Texture  = font->GetAtlasTexture(); // can be nullptr? (tmp: bind is false so doesnt matter)

				FontShader_->SetTexCoords(glm::vec4(cp.UVOffset + glm::vec2(0, cp.UVSize.y), cp.UVOffset + cp.UVSize),
				                          glm::vec4(cp.UVOffset, cp.UVOffset + glm::vec2(cp.UVSize.x, 0)));

				RenderQuad_(q, FontShader_, true); // shader already bound
			};
		}

		// Rend_->EnableDepthCheck();
	}

	static GuiGraphics *guiGrInst = nullptr;
	GuiGraphics *GuiGraphics::Instance()
	{
		if(guiGrInst == nullptr) guiGrInst = new GuiGraphics;
		return guiGrInst;
	}
	void GuiGraphics::SetInstance(GuiGraphics *newInstance) { guiGrInst = newInstance; }
} // namespace dcore::graphics::gui
