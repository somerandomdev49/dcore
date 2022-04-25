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
#include <glm/ext.hpp>

namespace dcore::graphics
{
	using namespace dcore::graphics::impl::opengl;

#define FOG_COLOR 0.23f, 0.48f, 0.74f, 1.0f

	void Renderer::OnBeginRender()
	{
		// TODO: ClearColor field.
		glClearColor(FOG_COLOR);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::EnableDepthCheck() { glEnable(GL_DEPTH_TEST); }
	void Renderer::DisableDepthCheck() { glDisable(GL_DEPTH_TEST); }

	void Renderer::OnEndRender() {}

	void Renderer::Initialize()
	{
		EnableDepthCheck();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Renderer::DeInitialize() {}

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

	void RenderResourceManager::CreateStaticMesh(RStaticMesh *mesh, const std::vector<uint32_t> &indices,
	                                             const std::vector<byte> &vertexData)
	{
		if(mesh == nullptr) return;
		mesh->Data_ = new impl::opengl::Vao();
		auto *vao = (impl::opengl::Vao *)mesh->Data_;
		vao->Load(indices, vertexData, sizeof(float) * (3 + 3 + 2));
		vao->CreateFloatAttribute(3); // Position
		vao->CreateFloatAttribute(3); // Normal
		vao->CreateFloatAttribute(2); // TexCoord
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

	void RenderResourceManager::DeleteTexture(RTexture *t)
	{
		auto *data = (impl::opengl::Texture *)t->Data_;
		data->Buffer.Delete();
		delete data;
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
