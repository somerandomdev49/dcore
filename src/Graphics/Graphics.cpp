#include <dcore/Core/Assert.hpp>
#include <dcore/Graphics/Graphics.hpp>
#include <dcore/Resource/ResourceManager.hpp>
#include <dcore/Graphics/Renderable.hpp>
#include <dcore/Graphics/Shader.hpp>
#include <dcore/Graphics/StaticMesh.hpp>
#include <dcore/Graphics/Camera.hpp>

using namespace dcore::graphics;

Shader::Shader(const resource::Resource<fwdraw::Shader> &sh)
    : Shader_(sh), UTransform_(sh.Get(), "u_Transform")
{
    DCORE_ASSERT_RETURN(sh.Get() != nullptr, "Bad Shader!");
}

fwdraw::Shader *Shader::Get() const { return Shader_.Get(); }
void Shader::SetTransform(const glm::mat4 &m) { UTransform_.set(m); }

const glm::mat4 &Renderable::GetTransform() const { return Transform_; }
void Renderable::SetTransform(const glm::mat4 &m) { Transform_ = m; }



StaticMesh::StaticMesh(const resource::Resource<fwdraw::Mesh> &mesh, const resource::Resource<fwdraw::Texture> &texture)
    : Mesh_(mesh), Texture_(texture) { }

const dcore::resource::Resource<fwdraw::Mesh> &StaticMesh::GetMesh() const { return Mesh_; }
const dcore::resource::Resource<fwdraw::Texture> &StaticMesh::GetTexture() const { return Texture_; }

Camera::Camera(float fov, float aspect, float near, float far)
{
    Fov_ = fov;
    Aspect_ = aspect;
    NearZ_ = near;
    FarZ_ = far;
    Position_ = glm::vec3(0.f, 1.f, -1.f);
    Rotation_ = glm::identity<glm::quat>();
    DirtyProj_ = true;
    DirtyView_ = true;
    RecalcProjMatrix();
    RecalcViewMatrix();
}

const glm::mat4 &Camera::GetViewMatrix() { if(DirtyView_) RecalcViewMatrix(); return ViewMatrix_; }
const glm::mat4 &Camera::GetProjMatrix() { if(DirtyProj_) RecalcProjMatrix(); return ProjMatrix_; }

float Camera::GetFov() const { return Fov_;  }
float Camera::GetAspectRatio() const { return Aspect_;  }
float Camera::GetNearZ() const { return NearZ_;  }
float Camera::GetFarZ() const { return FarZ_;  }

void Camera::SetFov(float newFov) { Fov_ = newFov; DirtyProj_ = true; }
void Camera::SetAspectRatio(float newAspectRatio) { Aspect_ = newAspectRatio; DirtyProj_ = true; }
void Camera::SetNearZ(float newNearZ) { NearZ_ = newNearZ; DirtyProj_ = true; }
void Camera::SetFarZ(float newFarZ) { FarZ_ = newFarZ; DirtyProj_ = true; }

const glm::vec3 &Camera::GetPosition() const { return Position_; }
void Camera::SetPosition(const glm::vec3 &newPosition) { Position_ = newPosition; DirtyView_ = true; }

const glm::quat &Camera::GetRotation() const { return Rotation_; }
void Camera::SetRotation(const glm::quat &newRotation) { Rotation_ = newRotation; DirtyView_ = true; }

void Camera::RecalcViewMatrix()
{
    // ViewMatrix_ = glm::mat4(1.0f);
    ViewMatrix_ = glm::mat4_cast(Rotation_);
    ViewMatrix_ = glm::translate(ViewMatrix_, Position_);
    ViewMatrix_ = glm::inverse(ViewMatrix_);
    DirtyView_ = false;
}

void Camera::RecalcProjMatrix()
{
    ProjMatrix_ = glm::perspective(Fov_, Aspect_, NearZ_, FarZ_);
    DirtyProj_ = false;
}


void RendererInterface::Initialize(resource::ResourceManager DCORE_REF *rm, Renderer DCORE_REF *rend)
{
    // TODO: Should not be hard-written!
    auto s = rm->Get<fwdraw::Shader>("DCore.Shader.ObjectShader");
    ObjectShader_ = new Shader(s);
    Camera_ = new Camera();
    
    Renderer_ = rend;
}

void RendererInterface::DeInitialize()
{
    delete ObjectShader_;
    delete Camera_;
}

Renderer *RendererInterface::GetRenderer() const { return Renderer_; }
Camera *RendererInterface::GetCamera() const { return Camera_; }
// TODO: SetCamera, make the camera DCORE_REF.

void RendererInterface::RenderStaticMesh(const StaticMesh DCORE_REF *sm)
{
    // TODO: Do not set the shader each time we render something, it's expensive!
    Renderer_->Get().bind_shader(ObjectShader_->Get()); // TODO: stay away from calling fwdraw in higher order classes.
    ObjectShader_->SetTransform(Camera_->GetProjMatrix() * Camera_->GetViewMatrix() * sm->GetTransform());
    Renderer_->Render(ObjectShader_->Get(), sm->GetMesh().Get(), sm->GetTexture().Get());
}
