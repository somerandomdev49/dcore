#include "GL/gl3w.h"
#include "GL/glcorearb.h"
#include "dcore/Renderer/RSkyBox.hpp"
#include <dcore/Renderer/Impl/OpenGL/GL.hpp>
#include <dcore/Resource/ResourceLoader.hpp>
#include <dcore/Renderer/Renderer.hpp>
#include <dcore/Util/LoaderUtil.hpp>
#include <dcore/Core/Assert.hpp>
#include <dcore/Core/Log.hpp>
#include <dcore/Renderer/Impl/OpenGL/Texture.hpp>
#include <dcore/Renderer/Impl/OpenGL/ShaderProgram.hpp>
#include <dcore/Renderer/Impl/OpenGL/Vao.hpp>
#include <dcore/Renderer/Impl/OpenGL/FastVao.hpp>
#include <dcore/Renderer/Impl/OpenGL/Framebuffer.hpp>
#include <glm/ext.hpp>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

namespace dcore::graphics
{
	using namespace dcore::graphics::impl::opengl;
	namespace details
	{
		struct Data
		{
			Framebuffer FB;
		};	
	}

	void Renderer::OnBeginRender()
	{
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

		glClearColor(ClearColor_.r, ClearColor_.g, ClearColor_.b, ClearColor_.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if(ShouldRenderToFB_)
		{
			Data_->FB.Bind();
			glClearColor(ClearColor_.r, ClearColor_.g, ClearColor_.b, ClearColor_.a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		glViewport(0, 0, ViewportSize_.x, ViewportSize_.y);
	}

	void Renderer::SetViewport(glm::vec2 size)
	{
		ViewportSize_ = size;
		Data_->FB.ReSize(ViewportSize_);
	}

	void Renderer::EnableDepthCheck() { glEnable(GL_DEPTH_TEST); }
	void Renderer::DisableDepthCheck() { glDisable(GL_DEPTH_TEST); }

	void Renderer::OnEndRender()
	{
		Data_->FB.UnBind();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void Renderer::Initialize()
	{
		ViewportSize_ = glm::vec2(2, 2);
		EnableDepthCheck();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    	ImGui_ImplOpenGL3_Init("#version 330 core");

		Data_ = new details::Data();
		Data_->FB.Create(ViewportSize_);
	}

	void Renderer::DeInitialize()
	{
		Data_->FB.Delete();
		delete Data_;
		ImGui_ImplOpenGL3_Shutdown();
	}

	void Renderer::UseTexture(int unit, RTexture *texture)
	{
		if(texture == nullptr) return;
		auto *data = (impl::opengl::TextureBuffer *)texture->Data_;
		Gl::SetActiveTexture(TextureUnit0 + unit);
		Gl::BindTexture(data->Type_, data->Id_);
	}

	void Renderer::Render(RStaticMesh *mesh)
	{
		// TODO: Use GL_TRIANGLE_STRIP instead of triangles. Will make the loading process more
		// complicated, but will decrease memory usage.

		auto *vao = (impl::opengl::Vao *)mesh->Data_;
		Gl::BindVertexArray(vao->VAO_);
		Gl::DrawElements(ElementTriangles, vao->IndexCount_, TypeUnsignedInt);
	}

	void Renderer::Render(RFastVertexBuffer *buf)
	{
		auto *vao = (impl::opengl::FastVao *)buf->Data_;
		Gl::BindVertexArray(vao->VAO_);
		Gl::DrawArrays(ElementTriangleStrip, vao->IndexCount_);
	}

	void Renderer::SetUniform(const RUniform &u, float v)
	{
		const auto *uloc = reinterpret_cast<const impl::opengl::UInt *>(&u.Data_);
		Gl::SetUniform(*uloc, v);
	}
	void Renderer::SetUniform(const RUniform &u, int v)
	{
		const auto *uloc = reinterpret_cast<const impl::opengl::UInt *>(&u.Data_);
		Gl::SetUniform(*uloc, v);
	}
	void Renderer::SetUniform(const RUniform &u, const glm::vec2 &v)
	{
		const auto *uloc = reinterpret_cast<const impl::opengl::UInt *>(&u.Data_);
		Gl::SetUniform(*uloc, v);
	}
	void Renderer::SetUniform(const RUniform &u, const glm::vec3 &v)
	{
		const auto *uloc = reinterpret_cast<const impl::opengl::UInt *>(&u.Data_);
		Gl::SetUniform(*uloc, v);
	}
	void Renderer::SetUniform(const RUniform &u, const glm::vec4 &v)
	{
		const auto *uloc = reinterpret_cast<const impl::opengl::UInt *>(&u.Data_);
		Gl::SetUniform(*uloc, v);
	}
	void Renderer::SetUniform(const RUniform &u, const glm::mat2x2 &v)
	{
		const auto *uloc = reinterpret_cast<const impl::opengl::UInt *>(&u.Data_);
		Gl::SetUniform(*uloc, v);
	}
	void Renderer::SetUniform(const RUniform &u, const glm::mat2x3 &v)
	{
		const auto *uloc = reinterpret_cast<const impl::opengl::UInt *>(&u.Data_);
		Gl::SetUniform(*uloc, v);
	}
	void Renderer::SetUniform(const RUniform &u, const glm::mat2x4 &v)
	{
		const auto *uloc = reinterpret_cast<const impl::opengl::UInt *>(&u.Data_);
		Gl::SetUniform(*uloc, v);
	}
	void Renderer::SetUniform(const RUniform &u, const glm::mat3x2 &v)
	{
		const auto *uloc = reinterpret_cast<const impl::opengl::UInt *>(&u.Data_);
		Gl::SetUniform(*uloc, v);
	}
	void Renderer::SetUniform(const RUniform &u, const glm::mat3x3 &v)
	{
		const auto *uloc = reinterpret_cast<const impl::opengl::UInt *>(&u.Data_);
		Gl::SetUniform(*uloc, v);
	}
	void Renderer::SetUniform(const RUniform &u, const glm::mat3x4 &v)
	{
		const auto *uloc = reinterpret_cast<const impl::opengl::UInt *>(&u.Data_);
		Gl::SetUniform(*uloc, v);
	}
	void Renderer::SetUniform(const RUniform &u, const glm::mat4x2 &v)
	{
		const auto *uloc = reinterpret_cast<const impl::opengl::UInt *>(&u.Data_);
		Gl::SetUniform(*uloc, v);
	}
	void Renderer::SetUniform(const RUniform &u, const glm::mat4x3 &v)
	{
		const auto *uloc = reinterpret_cast<const impl::opengl::UInt *>(&u.Data_);
		Gl::SetUniform(*uloc, v);
	}
	void Renderer::SetUniform(const RUniform &u, const glm::mat4x4 &v)
	{
		const auto *uloc = reinterpret_cast<const impl::opengl::UInt *>(&u.Data_);
		Gl::SetUniform(*uloc, v);
	}

	RUniform Renderer::GetUniform(RShader *shader, const char *name)
	{
		RUniform u;
		// ? Hack, requires that sizeof(void*) <= sizeof(impl::opengl::UInt), which is true on normal platforms
		auto *uloc = reinterpret_cast<impl::opengl::UInt *>(&u.Data_);

		*uloc = Gl::GetUniformLocation(((ShaderProgram *)shader->Data_)->Id_, name);
		return u;
	}
	
	void Renderer::DepthTestFunction(DepthTestFunc func)
	{
		static GLenum translation[] = { GL_LESS, GL_LEQUAL };
		glDepthFunc(translation[func]);
	}

	void Renderer::UseShader(RShader *shader) { Gl::UseProgram(((ShaderProgram *)shader->Data_)->Id_); }

	void Renderer::SetWireframeMode(bool newIsWireframeMode)
	{
		IsWireframeMode_ = newIsWireframeMode;
		if(IsWireframeMode_) Gl::SetPolygonMode(FrontAndBackFaces, PolygonModeLine);
		else Gl::SetPolygonMode(FrontAndBackFaces, PolygonModeFill);
	}

	void Renderer::RShader_Constructor(const std::string &path, void *placement)
	{
		auto *shader = new(placement) RShader();

		std::string vertexSource, fragmentSource;
		bool success = true;
		success &= util::LoaderUtil::LoadFile(vertexSource, path + ".vert");
		success &= util::LoaderUtil::LoadFile(fragmentSource, path + ".frag");
		if(!success)
		{
			DCORE_LOG_ERROR << "Failed to load shader source! " << path;
			return;
		}

		shader->Data_ = new impl::opengl::ShaderProgram();
		auto *program  = (impl::opengl::ShaderProgram *)shader->Data_;
		program->Create();
		program->AttachShader(impl::opengl::VertexShader, vertexSource);
		program->AttachShader(impl::opengl::FragmentShader, fragmentSource);
		program->Link();
	}

	void Renderer::RShader_DeConstructor(void *placement)
	{
		auto *shader = reinterpret_cast<RShader *>(placement);
		auto *program    = (impl::opengl::ShaderProgram *)shader->Data_;
		program->Delete();
		delete program;
		delete shader;
	}

	void *Renderer::GetFramebufferData()
	{
		return &Data_->FB.Texture_;
	}

	void RenderResourceManager::CreateStaticMesh(RStaticMesh *mesh, dstd::Span<const uint32_t> indices, dstd::Span<const byte> vertexData,
		const std::string &format)
	{
		if(mesh == nullptr) return;
		mesh->Data_ = new impl::opengl::Vao();
		auto *vao = (impl::opengl::Vao *)mesh->Data_;
		dstd::USize size = 0;
		for(char c : format)
			switch(c)
			{
			case 'p': size += 3; break;
			case 'n': size += 3; break;
			case 't': size += 2; break;
			}
		
		vao->Load(indices.Data, indices.Count, vertexData.Data, vertexData.Count, sizeof(float) * size);

		for(char c : format)
			switch(c)
			{
			case 'p': vao->CreateFloatAttribute(3); break;
			case 'n': vao->CreateFloatAttribute(3); break;
			case 't': vao->CreateFloatAttribute(2); break;
			}
	}

	void RenderResourceManager::CreateStaticMesh(RStaticMesh *mesh, const std::vector<uint32_t> &indices, dstd::Span<const byte> vertexData,
		const std::string &format)
	{
		CreateStaticMesh(mesh, { indices.data(), indices.size() }, vertexData, format);
	}


	void RenderResourceManager::CreateStaticMesh(RStaticMesh *mesh, const std::vector<uint32_t> &indices,
	                                             const std::vector<byte> &vertexData, const std::string &format)
	{
		CreateStaticMesh(mesh, { indices.data(), indices.size() }, { vertexData.data(), vertexData.size() }, format);
	}

	void RenderResourceManager::DeleteStaticMesh(RStaticMesh *mesh)
	{
		if(mesh == nullptr) return;
		auto *vao = (impl::opengl::Vao *)mesh->Data_;
		vao->Delete();
		delete vao; // Important: Leave as vao and not mesh->Data_ (because the latter is void*)
	}

	void RenderResourceManager::CreateTexture(RTexture *tex, byte *data, const glm::ivec2 &size, TextureFormat format,
	                                          TextureScaling scaling, int alignment)
	{
		namespace gl = impl::opengl;
		// RRM::TextureFormat -> opengl::TextureFormat
		int prevAlignment;
		if(alignment > 0)
		{
			prevAlignment = gl::Gl::GetPixelStore(gl::PixelStorageUnpackAlignment);
			gl::Gl::PixelStore(gl::PixelStorageUnpackAlignment, alignment);
		}
		static gl::TextureFormat formats[4] = {gl::TextureFormatR, gl::TextureFormatRg, gl::TextureFormatRgb,
		                                       gl::TextureFormatRgba};

		static gl::TextureParamValue filtersMin[2] = {gl::TextureFilterMipmapLinear, gl::TextureFilterNearest};
		static gl::TextureParamValue filtersMag[2] = {gl::TextureFilterLinear, gl::TextureFilterNearest};

		tex->Data_     = new gl::Texture();
		auto *t = (gl::Texture *)tex->Data_;
		t->Format      = gl::TextureFormatRgba;
		t->Size        = size;

		t->Buffer.Generate(gl::Texture2D);

		t->Buffer.SetParam(gl::TextureParamWrapS, gl::TextureWrapRepeat);
		t->Buffer.SetParam(gl::TextureParamWrapT, gl::TextureWrapRepeat);
		t->Buffer.SetParam(gl::TextureParamMinFilter, filtersMin[static_cast<int>(scaling)]);
		t->Buffer.SetParam(gl::TextureParamMagFilter, filtersMag[static_cast<int>(scaling)]);

		t->Buffer.LoadData(gl::TextureFormatRgba, size, formats[static_cast<int>(format)], data);
		t->Buffer.GenMipmaps();

		if(alignment > 0) gl::Gl::PixelStore(gl::PixelStorageUnpackAlignment, prevAlignment);
	}

	void RenderResourceManager::CreateSkyBox(RSkyBox *box, dstd::Span<byte*> datas, const glm::ivec2 &size, TextureFormat format,
	                                         TextureScaling scaling, int alignment)
	{
		namespace gl = impl::opengl;
		// RRM::TextureFormat -> opengl::TextureFormat
		int prevAlignment;
		if(alignment > 0)
		{
			prevAlignment = gl::Gl::GetPixelStore(gl::PixelStorageUnpackAlignment);
			gl::Gl::PixelStore(gl::PixelStorageUnpackAlignment, alignment);
		}
		static gl::TextureFormat formats[4] = {gl::TextureFormatR, gl::TextureFormatRg, gl::TextureFormatRgb,
		                                       gl::TextureFormatRgba};

		static gl::TextureParamValue filtersMin[2] = {gl::TextureFilterMipmapLinear, gl::TextureFilterNearest};
		static gl::TextureParamValue filtersMag[2] = {gl::TextureFilterLinear, gl::TextureFilterNearest};

		box->Data_ = new gl::Texture();
		auto *t = (gl::Texture *)box->Data_;
		t->Format = gl::TextureFormatRgba;
		t->Size = size;

		t->Buffer.Generate(gl::TextureCubemap);
		for(int i = 0; i < 6; ++i)
			t->Buffer.LoadData(
				(gl::TextureType)(gl::TextureCubemapPosX + i),
				gl::TextureFormatRgba, size, formats[static_cast<int>(format)], datas.Data[i]
			);
		// TODO: World:: fix warning, then add functions for changing skybox, changing fog. then make skybox store color, change fog
		t->Buffer.SetParam(gl::TextureParamWrapS, gl::TextureWrapClampToEdge);
		t->Buffer.SetParam(gl::TextureParamWrapT, gl::TextureWrapClampToEdge);
		t->Buffer.SetParam(gl::TextureParamWrapR, gl::TextureWrapClampToEdge);
		t->Buffer.SetParam(gl::TextureParamMinFilter, filtersMin[static_cast<int>(scaling)]);
		t->Buffer.SetParam(gl::TextureParamMagFilter, filtersMag[static_cast<int>(scaling)]);
		t->Buffer.GenMipmaps();

		if(alignment > 0) gl::Gl::PixelStore(gl::PixelStorageUnpackAlignment, prevAlignment);
	}

	void RenderResourceManager::DeleteTexture(RTexture *t)
	{
		if(t->Data_ == nullptr)
		{
			DCORE_LOG_WARNING << "DeleteTexture t->Data_ == nullptr";
			return;
		}
		auto *data = (impl::opengl::Texture *)t->Data_;
		data->Buffer.Delete();
		delete data;
		// delete data;
	}

	void RenderResourceManager::DeleteSkyBox(RSkyBox *t)
	{
		if(t->Data_ == nullptr)
		{
			DCORE_LOG_WARNING << "DeleteSkyBox t->Data_ == nullptr";
			return;
		}
		auto *data = (impl::opengl::Texture *)t->Data_;
		data->Buffer.Delete();
		delete data;
		// delete data;
	}

	void RenderResourceManager::CreateFastVertexBuffer(RFastVertexBuffer *buf, size_t indexCount)
	{
		buf->Data_ = new impl::opengl::FastVao;
		auto *data  = (impl::opengl::FastVao *)buf->Data_;
		data->Load(indexCount);
	}

	void RenderResourceManager::DeleteFastVertexBuffer(RFastVertexBuffer *buf)
	{
		auto *data = (impl::opengl::FastVao *)buf->Data_;
		data->Delete();
	}
} // namespace dcore::graphics
