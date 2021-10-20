#include <dcore/Graphics/GUI/GuiGraphics.hpp>
#include <dcore/Renderer/Renderer.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
using namespace dcore::graphics::gui;

/**************************** GuiShader ****************************/

GuiShader::GuiShader(const dcore::resource::Resource<dcore::graphics::RShader> &shader)
{
	Shader_ = shader;
	UTransform_ = Renderer::Instance()->GetUniform(Shader_.Get(), "u_Transform");
	UTex_ = Renderer::Instance()->GetUniform(Shader_.Get(), "u_Tex");
	UColor_ = Renderer::Instance()->GetUniform(Shader_.Get(), "u_Color");
	SetTexture(0);
}

void GuiShader::SetColor(const glm::vec4 &v) { Renderer::Instance()->SetUniform(UColor_, v); }
void GuiShader::SetTransform(const glm::mat3 &v) { Renderer::Instance()->SetUniform(UTransform_, v); }
void GuiShader::SetTexture(int unit) { Renderer::Instance()->SetUniform(UColor_, unit); }
dcore::graphics::RShader *GuiShader::Get() const { return Shader_.Get(); }

/**************************** GuiGraphics ****************************/

void GuiGraphics::Initialize(resource::ResourceManager DCORE_REF *rm, Renderer DCORE_REF *rend)
{
	if(rend == nullptr) rend = Renderer::Instance();
	Rend_ = rend;

	GuiShader_ = new GuiShader(rm->Get<RShader>("DCore.Shader.GuiShader"));
	Quad_      = new RFastVertexBuffer();
	RenderResourceManager::CreateFastVertexBuffer(Quad_, 4);
}

void GuiGraphics::DeInitialize()
{
	delete GuiShader_;
	RenderResourceManager::DeleteFastVertexBuffer(Quad_);
	delete Quad_;
}

void GuiGraphics::RenderQuad(const Quad &quad)
{
	if(GuiShader_) Rend_->UseShader(GuiShader_->Get());
	if(quad.Texture) Rend_->UseTexture(0, quad.Texture);

	glm::mat3 tr = glm::translate(glm::mat3(1.0f), quad.Position);
	tr           = glm::rotate(tr, quad.Rotation);
	tr           = glm::scale(tr, quad.Scale);
	GuiShader_->SetTransform(tr);
	GuiShader_->SetColor(quad.Color);
	// Texture is 0 by default.

	Rend_->Render(Quad_);
}

static GuiGraphics *guiGrInst = nullptr;
GuiGraphics *GuiGraphics::Instance() { if(guiGrInst == nullptr) guiGrInst = new GuiGraphics; return guiGrInst; }
void GuiGraphics::SetInstance(GuiGraphics *newInstance) { guiGrInst = newInstance; }
