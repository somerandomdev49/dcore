#include <dcore/Core/Assert.hpp>
#include <dcore/Graphics/Graphics.hpp>
#include <dcore/Resource/ResourceManager.hpp>
#include <dcore/Graphics/Renderable.hpp>
#include <dcore/Graphics/Shader.hpp>
#include <dcore/Graphics/StaticMesh.hpp>
#include <dcore/Graphics/Camera.hpp>
#include <dcore/Renderer/Renderer.hpp>
#include <dcore/Core/Preferences.hpp>
#include <glm/ext.hpp>

// TODO: Separate these into different files

namespace
{
	// https://stackoverflow.com/a/49824672/9110517
	glm::quat safeQuatLookAt(glm::vec3 const &lookFrom, glm::vec3 const &lookTo, glm::vec3 const &up,
	                         glm::vec3 const &alternativeUp)
	{
		glm::vec3 direction   = lookTo - lookFrom;
		float directionLength = glm::length(direction);

		if(!(directionLength > 0.0001)) return {1, 0, 0, 0}; // Just return identity

		// Normalize direction
		direction /= directionLength;

		// Is the normal up (nearly) parallel to direction?
		if(glm::abs(glm::dot(direction, up)) > .9999f)
		{
			return glm::quatLookAt(direction, alternativeUp);
		}

		return glm::quatLookAt(direction, up);
	}
} // namespace

namespace dcore::graphics
{
	/**************************** CommonShader ****************************/

	CommonShader::CommonShader(const resource::Resource<RShader> &sh) : Shader_(sh)
	{
		DCORE_ASSERT_RETURN(sh.Get() != nullptr, "Bad Shader!");
		UTransform_ = Renderer::Instance()->GetUniform(sh.Get(), "u_Transform");
		UFogColor_  = Renderer::Instance()->GetUniform(sh.Get(), "u_FogColor");
		UTex_       = Renderer::Instance()->GetUniform(sh.Get(), "u_Tex");
		Renderer::Instance()->SetUniform(UTex_, 0);
	}

	RShader *CommonShader::Get() const { return Shader_.Get(); }
	void CommonShader::SetTransform(const glm::mat4 &m) { Renderer::Instance()->SetUniform(UTransform_, m); }
	void CommonShader::SetFogColor(glm::vec3 m) { Renderer::Instance()->SetUniform(UFogColor_, m); }

	/**************************** TerrainShader ****************************/

	TerrainShader::TerrainShader(const resource::Resource<RShader> &sh) : Shader_(sh)
	{
		DCORE_ASSERT_RETURN(sh.Get() != nullptr, "Bad Shader!");
		UTransform_ = Renderer::Instance()->GetUniform(sh.Get(), "u_Transform");
		UFogColor_  = Renderer::Instance()->GetUniform(sh.Get(), "u_FogColor");

		UBlendMapTex_ = Renderer::Instance()->GetUniform(sh.Get(), "u_BlendMap");
		Renderer::Instance()->SetUniform(UBlendMapTex_, 0); // default values

		static const char *texNames[4] = {"u_Tex0", "u_TexR", "u_TexG", "u_TexB"};
		for(int i = 0; i < 4; ++i)
		{
			UTexs_[i] = Renderer::Instance()->GetUniform(sh.Get(), texNames[i]);
			Renderer::Instance()->SetUniform(UTexs_[i], i + 1); // default values
		}
	}

	RShader *TerrainShader::Get() const { return Shader_.Get(); }
	void TerrainShader::SetFogColor(glm::vec3 m) { Renderer::Instance()->SetUniform(UFogColor_, m); }
	void TerrainShader::SetTransform(const glm::mat4 &m) { Renderer::Instance()->SetUniform(UTransform_, m); }
	void TerrainShader::SetTextures(int blend, int n, int r, int g, int b)
	{
		if(blend != -1) Renderer::Instance()->SetUniform(UBlendMapTex_, blend);
		if(n != -1) Renderer::Instance()->SetUniform(UTexs_[0], n);
		if(r != -1) Renderer::Instance()->SetUniform(UTexs_[1], r);
		if(g != -1) Renderer::Instance()->SetUniform(UTexs_[2], g);
		if(b != -1) Renderer::Instance()->SetUniform(UTexs_[3], b);
	}

	/**************************** Renderable ****************************/

	const glm::mat4 &Renderable::GetTransform() const { return Transform_; }
	void Renderable::SetTransform(const glm::mat4 &martrix) { Transform_ = martrix; }

	/**************************** StaticMesh ****************************/

	StaticMesh::StaticMesh(const resource::Resource<RStaticMesh> &mesh, const resource::Resource<RTexture> &texture)
	    : Mesh_(mesh), Texture_(texture)
	{
	}

	const dcore::resource::Resource<RStaticMesh> &StaticMesh::GetMesh() const { return Mesh_; }
	const dcore::resource::Resource<RTexture> &StaticMesh::GetTexture() const { return Texture_; }

	/**************************** Camera ****************************/

	Camera::Camera(float fov, float aspect, float near, float far)
	{
		Fov_       = fov;
		Aspect_    = aspect;
		NearZ_     = near;
		FarZ_      = far;
		Position_  = glm::vec3(0.f, 1.f, -1.f);
		Rotation_  = glm::identity<glm::quat>();
		DirtyProj_ = true;
		DirtyView_ = true;
		RecalcProjMatrix();
		RecalcViewMatrix();
	}

	const glm::mat4 &Camera::GetViewMatrix()
	{
		if(DirtyView_) RecalcViewMatrix();
		return ViewMatrix_;
	}

	const glm::mat4 &Camera::GetProjMatrix()
	{
		if(DirtyProj_) RecalcProjMatrix();
		return ProjMatrix_;
	}

	float Camera::GetFov() const { return Fov_; }
	float Camera::GetAspectRatio() const { return Aspect_; }
	float Camera::GetNearZ() const { return NearZ_; }
	float Camera::GetFarZ() const { return FarZ_; }

	void Camera::SetFov(float newFov)
	{
		Fov_       = newFov;
		DirtyProj_ = true;
	}

	void Camera::SetAspectRatio(float newAspectRatio)
	{
		Aspect_    = newAspectRatio;
		DirtyProj_ = true;
	}

	void Camera::SetNearZ(float newNearZ)
	{
		NearZ_     = newNearZ;
		DirtyProj_ = true;
	}

	void Camera::SetFarZ(float newFarZ)
	{
		FarZ_      = newFarZ;
		DirtyProj_ = true;
	}

	const glm::vec3 &Camera::GetPosition() const { return Position_; }
	void Camera::SetPosition(const glm::vec3 &newPosition)
	{
		Position_  = newPosition;
		DirtyView_ = true;
	}

	const glm::quat &Camera::GetRotation() const { return Rotation_; }
	void Camera::SetRotation(const glm::quat &newRotation)
	{
		Rotation_  = newRotation;
		DirtyView_ = true;
	}

	void Camera::RecalcViewMatrix()
	{
		// ViewMatrix_ = glm::mat4(1.0f);
		// ViewMatrix_ = glm::mat4_cast(Rotation_);
		// ViewMatrix_ = glm::translate(ViewMatrix_, Position_);
		// ViewMatrix_ = glm::inverse(ViewMatrix_);
		ViewMatrix_ = glm::lookAt(Position_, Position_ + Rotation_ * glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
		DirtyView_  = false;
	}

	void Camera::RecalcProjMatrix()
	{
		ProjMatrix_ = glm::perspective(glm::radians(Fov_), Aspect_, NearZ_, FarZ_);
		DirtyProj_  = false;
	}

	void Camera::LookAt(const glm::vec3 &position)
	{
		Rotation_ = safeQuatLookAt(Position_, position, glm::vec3(0, 1, 0), glm::vec3(1, 0, 0));
	}

	/**************************** RendererInterface ****************************/

	void RendererInterface::Initialize(resource::ResourceManager DCORE_REF *rm, Renderer DCORE_REF *rend)
	{
		Renderer_ = rend;

		// TODO: Should not be hard-written!
		auto sobj = rm->Get<RShader>("DCore.Shader.ObjectShader");
		auto ster = rm->Get<RShader>("DCore.Shader.TerrainShader");
		auto ssky = rm->Get<RShader>("DCore.Shader.SkyBoxShader");
		auto sskt = rm->Get<RShader>("DCore.Shader.SkyBoxTransShader");

		ObjectShader_  = new CommonShader(sobj);
		TerrainShader_ = new TerrainShader(ster);
		SkyBoxShader_ = new CommonShader(ssky);
		SkyBoxTransShader_ = new CommonShader(sskt);
		Renderer_->UseShader(sskt.Get());
		Renderer_->SetUniform(Renderer_->GetUniform(sskt.Get(), "u_Skybox1"), 0);
		Renderer_->SetUniform(Renderer_->GetUniform(sskt.Get(), "u_Skybox2"), 1);

		SkyBoxMesh_ = rm->Get<RStaticMesh>("DCore.Mesh.SkyBox").Get();

		glm::vec2 res = Renderer_->GetViewport();
		Camera_       = new Camera(Preferences::Instance()->GetGraphicsSettings().FOV, res.x / res.y, 0.1f, 500.0f);
		DCORE_LOG_INFO << "Camera FOV: " << Camera_->GetFov();
	}

	void RendererInterface::DeInitialize()
	{
		delete ObjectShader_;
		delete TerrainShader_;
		delete SkyBoxShader_;
		delete SkyBoxTransShader_;
		delete Camera_;
	}

	void RendererInterface::RenderStaticMesh(const StaticMesh DCORE_REF *mesh, const glm::mat4 &transform)
	{
		// TODO: Do not set the shader each time we render something, it's expensive!
		Renderer_->UseShader(ObjectShader_->Get());
		Renderer_->UseTexture(0, mesh->GetTexture().Get());
		ObjectShader_->SetTransform(Camera_->GetProjMatrix() * Camera_->GetViewMatrix() * transform);
		Renderer_->Render(mesh->GetMesh().Get());
	}

	void RendererInterface::RenderChunk(const terrain::Chunk *chunk)
	{
		Renderer_->UseShader(TerrainShader_->Get());

		auto globalPos = chunk->GetGlobalPosition();
		// printf("pos: %f, %f\n", p.x, p.y);
		glm::mat4 matrix = glm::translate(glm::mat4(1.0f), glm::vec3(globalPos.x, 0, globalPos.y));

		TerrainShader_->SetTransform(Camera_->GetProjMatrix() * Camera_->GetViewMatrix() * matrix); //  *

		Renderer_->UseTexture(0, chunk->GetBlendMap().Get());

		// if texs[n].Get() is nullptr, the call is skipped so we can safely use textures.
		const auto *texs = chunk->GetTextures();
		Renderer_->UseTexture(1, texs[0].Get());
		Renderer_->UseTexture(2, texs[1].Get());
		Renderer_->UseTexture(3, texs[2].Get());
		Renderer_->UseTexture(4, texs[3].Get());
		TerrainShader_->SetTextures(0, 1, 2, 3, 4);

		Renderer_->Render(chunk->GetMesh());
	}

	void RendererInterface::RenderModel(const Model DCORE_REF *model, const glm::mat4 &transform)
	{
		// TODO: Do not set the shader each time we render something, it's expensive!
		Renderer_->UseShader(ObjectShader_->Get());
		ObjectShader_->SetTransform(Camera_->GetProjMatrix() * Camera_->GetViewMatrix() * transform);
		Renderer_->Render(model);
	}

	void RendererInterface::RenderSkyBox(const RSkyBox DCORE_REF *skybox)
	{
		Renderer_->DepthTestFunction(graphics::Renderer::DepthTestFuncLEqual);
		Renderer_->UseShader(SkyBoxShader_->Get());
		SkyBoxShader_->SetTransform(Camera_->GetProjMatrix() * glm::mat4(glm::mat3(Camera_->GetViewMatrix())));
		Renderer_->UseTexture(0, (graphics::RTexture *)skybox); // TODO? (texture!=skybox?)
		Renderer_->Render(SkyBoxMesh_);
		Renderer_->DepthTestFunction(graphics::Renderer::DepthTestFuncLess);
	}

	void RendererInterface::SetFogColor(glm::vec3 newColor)
	{
		FogColor_ = newColor;
		ObjectShader_->SetFogColor(FogColor_);
		TerrainShader_->SetFogColor(FogColor_);
	}

	void RendererInterface::RenderSkyBox(const RSkyBox DCORE_REF *skybox1, const RSkyBox DCORE_REF *skybox2, float t)
	{
		Renderer_->DepthTestFunction(graphics::Renderer::DepthTestFuncLEqual);
		Renderer_->UseShader(SkyBoxTransShader_->Get());
		Renderer_->SetUniform(Renderer_->GetUniform(SkyBoxTransShader_->Get(), "u_Alpha"), t);
		SkyBoxTransShader_->SetTransform(Camera_->GetProjMatrix() * glm::mat4(glm::mat3(Camera_->GetViewMatrix())));
		Renderer_->UseTexture(0, (graphics::RTexture *)skybox1);
		Renderer_->UseTexture(1, (graphics::RTexture *)skybox2);
		Renderer_->Render(SkyBoxMesh_);
		Renderer_->DepthTestFunction(graphics::Renderer::DepthTestFuncLess);
	}
} // namespace dcore::graphics
