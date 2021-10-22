#include <dcore/Graphics/GUI/GuiGraphics.hpp>
#include <dcore/Graphics/GUI/Font.hpp>
#include <dcore/Renderer/Renderer.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
using namespace dcore::graphics::gui;

/**************************** GuiShader ****************************/

GuiShader::GuiShader(const dcore::resource::Resource<dcore::graphics::RShader> &shader)
{
	Shader_     = shader;
	UTransform_ = Renderer::Instance()->GetUniform(Shader_.Get(), "u_Transform");
	UTex_       = Renderer::Instance()->GetUniform(Shader_.Get(), "u_Tex");
	UColor_     = Renderer::Instance()->GetUniform(Shader_.Get(), "u_Color");
	SetTexture(0);
}

void GuiShader::SetColor(const glm::vec4 &v) { Renderer::Instance()->SetUniform(UColor_, v); }
void GuiShader::SetTransform(const glm::mat3 &v) { Renderer::Instance()->SetUniform(UTransform_, v); }
void GuiShader::SetTexture(int unit) { Renderer::Instance()->SetUniform(UColor_, unit); }
dcore::graphics::RShader *GuiShader::Get() const { return Shader_.Get(); }

/**************************** FontShader ****************************/

FontShader::FontShader(const dcore::resource::Resource<dcore::graphics::RShader> &shader)
{
	static const char *uniformNames[4] = { "u_TexCoords[0]", "u_TexCoords[1]", "u_TexCoords[2]", "u_TexCoords[3]" };

	Shader_     = shader;
	UTransform_ = Renderer::Instance()->GetUniform(Shader_.Get(), "u_Transform");
	UTex_       = Renderer::Instance()->GetUniform(Shader_.Get(), "u_Tex");
	UColor_     = Renderer::Instance()->GetUniform(Shader_.Get(), "u_Color");
	for(int i = 0; i < 4; ++i)
	{
		UTexCoords_[i] = Renderer::Instance()->GetUniform(Shader_.Get(), uniformNames[i]);
	}
	SetTexture(0);
}

void FontShader::SetColor(const glm::vec4 &v) { Renderer::Instance()->SetUniform(UColor_, v); }
void FontShader::SetTransform(const glm::mat3 &v) { Renderer::Instance()->SetUniform(UTransform_, v); }
void FontShader::SetTexture(int unit) { Renderer::Instance()->SetUniform(UColor_, unit); }
void FontShader::SetTexCoords(const glm::vec4 &s, const glm::vec4 &e)
{
	Renderer::Instance()->SetUniform(UTexCoords_[0], glm::vec2(s.x, s.y));
	Renderer::Instance()->SetUniform(UTexCoords_[1], glm::vec2(s.z, s.w));
	Renderer::Instance()->SetUniform(UTexCoords_[2], glm::vec2(e.x, e.y));
	Renderer::Instance()->SetUniform(UTexCoords_[3], glm::vec2(e.z, e.w));
}
dcore::graphics::RShader *FontShader::Get() const { return Shader_.Get(); }

/**************************** GuiGraphics ****************************/

void GuiGraphics::Initialize(resource::ResourceManager DCORE_REF *rm, Renderer DCORE_REF *rend)
{
	if(rend == nullptr) rend = Renderer::Instance();
	Rend_ = rend;

	GuiShader_ = new GuiShader(rm->Get<RShader>("DCore.Shader.GuiShader"));
	FontShader_ = new FontShader(rm->Get<RShader>("DCore.Shader.FontShader"));
	Quad_      = new RFastVertexBuffer();
	RenderResourceManager::CreateFastVertexBuffer(Quad_, 4);
}

void GuiGraphics::DeInitialize()
{
	delete GuiShader_;
	delete FontShader_;
	RenderResourceManager::DeleteFastVertexBuffer(Quad_);
	delete Quad_;
}

void GuiGraphics::RenderQuad_(const Quad &quad, RShader *shader)
{
	if(shader) Rend_->UseShader(shader);
	if(quad.Texture) Rend_->UseTexture(0, quad.Texture);

	auto tr = glm::translate(glm::mat3(1.0f), quad.Position);
	tr      = glm::rotate(tr, quad.Rotation);
	tr      = glm::scale(tr, quad.Scale);
	GuiShader_->SetTransform(tr);
	GuiShader_->SetColor(quad.Color);
	// Texture is 0 by default.

	Rend_->Render(Quad_);
}

// TODO: inline?
void GuiGraphics::RenderQuad(const Quad &quad) { RenderQuad_(quad, GuiShader_->Get()); }

void GuiGraphics::RenderText(Font *font, const char *text)
{
	if(!text) return;

	// Bind once!
	Rend_->UseShader(GuiShader_->Get());

	// char c = text[0];
	float currentX = 0.0f;
	while(*(text++))
	{
		if(*text >= 32)
		{
			auto &cp = font->CodePointTable_[*text];
			// FontShader_->SetTexCoords(
			// 	// glm::vec4(
			// 	// 	glm::vec2(cp.XOffset, cp.YOffset),
			// 	// 	glm::vec2(cp.XOffset + cp.Width, cp.YOffset)
			// 	// ),
			// 	// glm::vec4(
			// 	// 	glm::vec2(cp.XOffset, cp.YOffset + cp.Height),
			// 	// 	glm::vec2(cp.XOffset + cp.Width, cp.YOffset + cp.Height)
			// 	// )
			// 	glm::vec4(glm::vec2(-1, 1), glm::vec2(1, 1)),
			// 	glm::vec4(glm::vec2(-1, -1), glm::vec2(1, -1))
			// );
			Quad q;
			q.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			q.Position = glm::vec2(0.0f, 0.0f);
			// q.Scale = glm::vec2(0.1f, (float(cp.Width) / float(cp.Height) * 0.1f));
			q.Scale = glm::vec2(1.0f, 1.0f);
			q.Rotation = 0.0f;
			q.Texture = font->GetAtlasTexture(); // can be nullptr?
			RenderQuad_(q, GuiShader_->Get()); // shader already bound
			currentX += 0.1f;
		}
	}
}

static GuiGraphics *guiGrInst = nullptr;
GuiGraphics *GuiGraphics::Instance()
{
	if(guiGrInst == nullptr) guiGrInst = new GuiGraphics;
	return guiGrInst;
}
void GuiGraphics::SetInstance(GuiGraphics *newInstance) { guiGrInst = newInstance; }
