#include <dcore/Renderer/Impl/OpenGL/GL.hpp>
#include <dcore/Resource/ResourceLoader.hpp>
#include <dcore/Renderer/Renderer.hpp>
#include <dcore/Util/LoaderUtil.hpp>
#include <dcore/Core/Assert.hpp>
#include <dcore/Core/Log.hpp>
#include <glm/ext.hpp>

using namespace dcore::graphics;
using namespace dcore::graphics::impl::opengl;

#define FOG_COLOR 0.2f, 0.4f, 0.5f, 1.0f

void Renderer::OnBeginRender()
{
	// TODO: ClearColor field.
	glClearColor(FOG_COLOR);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::OnEndRender() {}

void Renderer::Initialize() { glEnable(GL_DEPTH_TEST); }

void Renderer::DeInitialize() {}

void Renderer::UseTexture(int unit, RTexture *texture)
{
	if(!texture) return;
	Gl::SetActiveTexture(TextureUnit0 + unit);
	Gl::BindTexture(texture->Data_.Texture_.Type_, texture->Data_.Texture_.Id_);
}

void Renderer::Render(RStaticMesh *mesh)
{
	// TODO: Use GL_TRIANGLE_STRIP instead of triangles. Will make the loading process more complicated, but will decrease memory usage.
	// printf("%u indices, %u vao\n", mesh->Data_.Vao_.IndexCount_, mesh->Data_.Vao_.VAO_);
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

	static RRM::TextureFormat formats[4] = {RRM::TextureFormat::Red, RRM::TextureFormat::Rg, RRM::TextureFormat::Rgb, RRM::TextureFormat::Rgba};

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

void RenderResourceManager::CreateStaticMesh(RStaticMesh *mesh, const std::vector<uint32_t> &indices, const std::vector<byte> &vertexData)
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
void RenderResourceManager::CreateTexture(RTexture *tex, byte *data, const glm::ivec2 &size, TextureFormat format)
{
	// RRM::TextureFormat -> opengl::TextureFormat
	static impl::opengl::TextureFormat formats[4] = {impl::opengl::TextureFormatR, impl::opengl::TextureFormatRg, impl::opengl::TextureFormatRgb,
	                                                 impl::opengl::TextureFormatRgba};
	tex->Data_.Texture_.Generate(impl::opengl::Texture2D);

	tex->Data_.Texture_.SetParam(impl::opengl::TextureParamWrapS, impl::opengl::TextureWrapRepeat);
	tex->Data_.Texture_.SetParam(impl::opengl::TextureParamWrapT, impl::opengl::TextureWrapRepeat);
	tex->Data_.Texture_.SetParam(impl::opengl::TextureParamMinFilter, impl::opengl::TextureFilterMipmapLinear);
	tex->Data_.Texture_.SetParam(impl::opengl::TextureParamMagFilter, impl::opengl::TextureFilterLinear);

	tex->Data_.Texture_.LoadData(impl::opengl::TextureFormatRgba, size, formats[static_cast<int>(format)], data);
	tex->Data_.Texture_.GenMipmaps();
}

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