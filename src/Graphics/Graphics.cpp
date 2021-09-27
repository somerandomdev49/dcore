#include <dcore/Graphics/Graphics.hpp>
#include <dcore/Resource/ResourceManager.hpp>
#include <dcore/Graphics/Renderable.hpp>
#include <dcore/Graphics/Shader.hpp>
#include <dcore/Graphics/StaticMesh.hpp>

using namespace dcore::graphics;

Shader::Shader(const resource::Resource<fwdraw::Shader> &sh)
    : Shader_(sh), UTransform_(sh.Get(), "u_Transform") { }

fwdraw::Shader *Shader::Get() const { return Shader_.Get(); }
void Shader::SetTransform(const glm::mat4 &m) { UTransform_.set(m); }

const glm::mat4 &Renderable::GetTransform() const { return Transform_; }
void Renderable::SetTransform(const glm::mat4 &m) { Transform_ = m; }



StaticMesh::StaticMesh(const resource::Resource<fwdraw::Mesh> &mesh, const resource::Resource<fwdraw::Texture> &texture)
    : Mesh_(mesh), Texture_(texture) { }

const dcore::resource::Resource<fwdraw::Mesh> &StaticMesh::GetMesh() const { return Mesh_; }
const dcore::resource::Resource<fwdraw::Texture> &StaticMesh::GetTexture() const { return Texture_; }



void RendererInterface::Initialize(resource::ResourceManager DCORE_REF *rm, Renderer DCORE_REF *rend)
{
    // TODO: Should not be hard-written!
    ObjectShader_ = new Shader(rm->Get<fwdraw::Shader>("DCore.Shader.ObjectShader")); 
    Renderer_ = rend;
}

void RendererInterface::DeInitialize()
{
    delete ObjectShader_;
}

Renderer *RendererInterface::GetRenderer() const { return Renderer_; }

void RendererInterface::RenderStaticMesh(const StaticMesh DCORE_REF *sm)
{
    // TODO: Do not set the shader each time we render something, it's expensive!
    ObjectShader_->SetTransform(sm->GetTransform());
    Renderer_->Render(ObjectShader_->Get(), sm->GetMesh().Get(), sm->GetTexture().Get());
}
