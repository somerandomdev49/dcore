#include <dcore/Renderer/Impl/OpenGL/GL.hpp>
#include <dcore/Resource/ResourceLoader.hpp>
#include <dcore/Renderer/Renderer.hpp>
#include <dcore/Util/LoaderUtil.hpp>
#include <dcore/Core/Assert.hpp>
#include <dcore/Core/Log.hpp>
#include <glm/ext.hpp>

namespace dcore::graphics
{
	using namespace dcore::graphics::impl::opengl;

#define FOG_COLOR 0.517f, 0.686f, 0.850f, 1.0f

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
		if(!texture) return;
		Gl::SetActiveTexture(TextureUnit0 + unit);
		Gl::BindTexture(texture->Data_.Texture_.Type_, texture->Data_.Texture_.Id_);
	}

	void Renderer::Render(RStaticMesh *mesh)
	{
		// TODO: Use GL_TRIANGLE_STRIP instead of triangles. Will make the loading process more complicated, but will
		// decrease memory usage. printf("%u indices, %u vao\n", mesh->Data_.Vao_.IndexCount_, mesh->Data_.Vao_.VAO_);
		Gl::BindVertexArray(mesh->Data_.Vao_.VAO_);
		Gl::DrawElements(ElementTriangles, mesh->Data_.Vao_.IndexCount_, TypeUnsignedInt);
	}

	void Renderer::Render(RFastVertexBuffer *buf)
	{
		Gl::BindVertexArray(buf->Data_.Vao_.VAO_);
		Gl::DrawArrays(ElementTriangleStrip, buf->Data_.Vao_.IndexCount_);
	}

	void Renderer::SetUniform(const RUniform &u, float v) { Gl::SetUniform(u.Data_.Location, v); }
	void Renderer::SetUniform(const RUniform &u, int v) { Gl::SetUniform(u.Data_.Location, v); }
	void Renderer::SetUniform(const RUniform &u, const glm::vec2 &v) { Gl::SetUniform(u.Data_.Location, v); }
	void Renderer::SetUniform(const RUniform &u, const glm::vec3 &v) { Gl::SetUniform(u.Data_.Location, v); }
	void Renderer::SetUniform(const RUniform &u, const glm::vec4 &v) { Gl::SetUniform(u.Data_.Location, v); }
	void Renderer::SetUniform(const RUniform &u, const glm::mat2x2 &v) { Gl::SetUniform(u.Data_.Location, v); }
	void Renderer::SetUniform(const RUniform &u, const glm::mat2x3 &v) { Gl::SetUniform(u.Data_.Location, v); }
	void Renderer::SetUniform(const RUniform &u, const glm::mat2x4 &v) { Gl::SetUniform(u.Data_.Location, v); }
	void Renderer::SetUniform(const RUniform &u, const glm::mat3x2 &v) { Gl::SetUniform(u.Data_.Location, v); }
	void Renderer::SetUniform(const RUniform &u, const glm::mat3x3 &v) { Gl::SetUniform(u.Data_.Location, v); }
	void Renderer::SetUniform(const RUniform &u, const glm::mat3x4 &v) { Gl::SetUniform(u.Data_.Location, v); }
	void Renderer::SetUniform(const RUniform &u, const glm::mat4x2 &v) { Gl::SetUniform(u.Data_.Location, v); }
	void Renderer::SetUniform(const RUniform &u, const glm::mat4x3 &v) { Gl::SetUniform(u.Data_.Location, v); }
	void Renderer::SetUniform(const RUniform &u, const glm::mat4x4 &v) { Gl::SetUniform(u.Data_.Location, v); }

	RUniform Renderer::GetUniform(RShader *shader, const char *name)
	{
		RUniform u;
		u.Data_.Location = Gl::GetUniformLocation(shader->Data_.Program_.Id_, name);
		return u;
	}

	void Renderer::UseShader(RShader *shader) { Gl::UseProgram(shader->Data_.Program_.Id_); }

	void Renderer::SetWireframeMode(bool newIsWireframeMode)
	{
		IsWireframeMode_ = newIsWireframeMode;
		if(IsWireframeMode_) Gl::SetPolygonMode(FrontAndBackFaces, PolygonModeLine);
		else Gl::SetPolygonMode(FrontAndBackFaces, PolygonModeFill);
	}

	void Renderer::RTexture_Constructor(const std::string &path, void *placement)
	{
		using RRM = RenderResourceManager;

		static RRM::TextureFormat formats[4] = {RRM::TextureFormat::Red, RRM::TextureFormat::Rg,
		                                        RRM::TextureFormat::Rgb, RRM::TextureFormat::Rgba};

		// This constructs a RTexture at the specified address (see "placement new")
		RTexture *tex = new(placement) RTexture();

		util::ImageData d;
		DCORE_LOG_INFO << "loading image from " << path;
		util::LoaderUtil::LoadImage(d, path);
		if(d.data == nullptr)
		{
			DCORE_LOG_ERROR << "Failed to load texture at '" << path << "'";
			return;
		}
		auto fmt = formats[d.channels - 1];

		DCORE_LOG_INFO << "Generating image (" << d.channels << 'x' << d.size.x << 'x' << d.size.y << ')';
		RenderResourceManager::CreateTexture(tex, (byte *)d.data, d.size, fmt);

		free(d.data);
	}

	void Renderer::RTexture_DeConstructor(void *placement)
	{
		RTexture *tex = reinterpret_cast<RTexture *>(placement);
		tex->Data_.Texture_.Delete();
		delete tex;
	}

	void Renderer::RShader_Constructor(const std::string &path, void *placement)
	{
		RShader *shader = new(placement) RShader();

		std::string vertexSource, fragmentSource;
		bool success = true;
		success &= util::LoaderUtil::LoadFile(vertexSource, path + ".vert");
		success &= util::LoaderUtil::LoadFile(fragmentSource, path + ".frag");
		if(!success)
		{
			DCORE_LOG_ERROR << "Failed to load shader source! " << path;
			return;
		}

		shader->Data_.Program_.Create();
		shader->Data_.Program_.AttachShader(impl::opengl::VertexShader, vertexSource);
		shader->Data_.Program_.AttachShader(impl::opengl::FragmentShader, fragmentSource);
		shader->Data_.Program_.Link();
	}

	void Renderer::RShader_DeConstructor(void *placement)
	{
		RShader *shader = reinterpret_cast<RShader *>(placement);
		shader->Data_.Program_.Delete();
		delete shader;
	}

	void RenderResourceManager::CreateStaticMesh(RStaticMesh *mesh, const std::vector<uint32_t> &indices,
	                                             const std::vector<byte> &vertexData)
	{
		if(!mesh) return;
		mesh->Data_.Vao_.Load(indices, vertexData, sizeof(float) * (3 + 3 + 2));
		mesh->Data_.Vao_.CreateFloatAttribute(3); // Position
		mesh->Data_.Vao_.CreateFloatAttribute(3); // Normal
		mesh->Data_.Vao_.CreateFloatAttribute(2); // TexCoord
	}

	void RenderResourceManager::DeleteStaticMesh(RStaticMesh *mesh)
	{
		if(!mesh) return;
		mesh->Data_.Vao_.Delete();
	}

	// TODO: DeleteTexture
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

		tex->Data_.Texture_.Generate(gl::Texture2D);

		tex->Data_.Texture_.SetParam(gl::TextureParamWrapS, gl::TextureWrapRepeat);
		tex->Data_.Texture_.SetParam(gl::TextureParamWrapT, gl::TextureWrapRepeat);
		tex->Data_.Texture_.SetParam(gl::TextureParamMinFilter, filtersMin[static_cast<int>(scaling)]);
		tex->Data_.Texture_.SetParam(gl::TextureParamMagFilter, filtersMag[static_cast<int>(scaling)]);

		tex->Data_.Texture_.LoadData(gl::TextureFormatRgba, size, formats[static_cast<int>(format)], data);
		tex->Data_.Texture_.GenMipmaps();

		if(alignment > 0) gl::Gl::PixelStore(gl::PixelStorageUnpackAlignment, prevAlignment);
	}

	// TODO: USE THIS METHOD!!!!!!
	void RenderResourceManager::DeleteTexture(RTexture *texture) { texture->Data_.Texture_.Delete(); }

	void RenderResourceManager::CreateFastVertexBuffer(RFastVertexBuffer *buf, size_t indexCount)
	{
		// very short indeed.
		buf->Data_.Vao_.Load(indexCount);
	}

	void RenderResourceManager::DeleteFastVertexBuffer(RFastVertexBuffer *buf)
	{
		// also very short.
		buf->Data_.Vao_.Delete();
	}
} // namespace dcore::graphics
