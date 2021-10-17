#include <dcore/Renderer/Impl/OpenGL.hpp>
#include <dcore/Resource/ResourceLoader.hpp>
#include <dcore/Renderer/Renderer.hpp>
#include <dcore/Util/LoaderUtil.hpp>
#include <dcore/Core/Assert.hpp>
#include <dcore/Core/Log.hpp>
#include <glm/ext.hpp>

using namespace dcore::graphics;

void Renderer::OnBeginRender()
{
	glClearColor(.1f, .1f, .1f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::OnEndRender() {}

void Renderer::Initialize() { glEnable(GL_DEPTH_TEST); }

void Renderer::DeInitialize() {}

void Renderer::UseTexture(int unit, RTexture *texture)
{
	if(!texture) return;
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(texture->Data_.Texture_.Type_, texture->Data_.Texture_.Id_);
}

void Renderer::Render(RStaticMesh *mesh)
{
	// TODO: Use GL_TRIANGLE_STRIP instead of triangles. Will make the loading process more complicated, but will decrease memory usage.
	// printf("%u indices, %u vao\n", mesh->Data_.Vao_.IndexCount_, mesh->Data_.Vao_.VAO_);
	glBindVertexArray(mesh->Data_.Vao_.VAO_);
	glDrawElements(GL_TRIANGLES, mesh->Data_.Vao_.IndexCount_, GL_UNSIGNED_INT, 0);
}

void Renderer::SetUniform(const RUniform &u, float v) { glUniform1f(u.Data_.Location, v); }
void Renderer::SetUniform(const RUniform &u, int v) { glUniform1i(u.Data_.Location, v); }
void Renderer::SetUniform(const RUniform &u, const glm::vec2 &v) { glUniform2f(u.Data_.Location, v.x, v.y); }
void Renderer::SetUniform(const RUniform &u, const glm::vec3 &v) { glUniform3f(u.Data_.Location, v.x, v.y, v.z); }
void Renderer::SetUniform(const RUniform &u, const glm::vec4 &v) { glUniform4f(u.Data_.Location, v.x, v.y, v.z, v.w); }
void Renderer::SetUniform(const RUniform &u, const glm::mat2x2 &v) { glUniformMatrix2fv(u.Data_.Location, 1, GL_FALSE, glm::value_ptr(v)); }
void Renderer::SetUniform(const RUniform &u, const glm::mat2x3 &v) { glUniformMatrix2x3fv(u.Data_.Location, 1, GL_FALSE, glm::value_ptr(v)); }
void Renderer::SetUniform(const RUniform &u, const glm::mat2x4 &v) { glUniformMatrix2x4fv(u.Data_.Location, 1, GL_FALSE, glm::value_ptr(v)); }
void Renderer::SetUniform(const RUniform &u, const glm::mat3x2 &v) { glUniformMatrix3x2fv(u.Data_.Location, 1, GL_FALSE, glm::value_ptr(v)); }
void Renderer::SetUniform(const RUniform &u, const glm::mat3x3 &v) { glUniformMatrix3fv(u.Data_.Location, 1, GL_FALSE, glm::value_ptr(v)); }
void Renderer::SetUniform(const RUniform &u, const glm::mat3x4 &v) { glUniformMatrix3x4fv(u.Data_.Location, 1, GL_FALSE, glm::value_ptr(v)); }
void Renderer::SetUniform(const RUniform &u, const glm::mat4x2 &v) { glUniformMatrix4x2fv(u.Data_.Location, 1, GL_FALSE, glm::value_ptr(v)); }
void Renderer::SetUniform(const RUniform &u, const glm::mat4x3 &v) { glUniformMatrix4x3fv(u.Data_.Location, 1, GL_FALSE, glm::value_ptr(v)); }
void Renderer::SetUniform(const RUniform &u, const glm::mat4x4 &v) { glUniformMatrix4fv(u.Data_.Location, 1, GL_FALSE, glm::value_ptr(v)); }

RUniform Renderer::GetUniform(RShader *shader, const char *name)
{
	RUniform u;
	u.Data_.Location = glGetUniformLocation(shader->Data_.Program_.Id_, name);
	return u;
}

void Renderer::UseShader(RShader *shader) { glUseProgram(shader->Data_.Program_.Id_); }

void Renderer::SetWireframeMode(bool newIsWireframeMode)
{
	IsWireframeMode_ = newIsWireframeMode;
	if(IsWireframeMode_) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::RTexture_Constructor(const std::string &path, void *placement)
{
	static impl::opengl::TextureFormat formats[4] = {impl::opengl::TextureFormatR, impl::opengl::TextureFormatRg, impl::opengl::TextureFormatRgb,
	                                                 impl::opengl::TextureFormatRgba};

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

	tex->Data_.Texture_.Generate(impl::opengl::Texture2D);

	tex->Data_.Texture_.SetParam(impl::opengl::TextureParamWrapS, impl::opengl::TextureWrapRepeat);
	tex->Data_.Texture_.SetParam(impl::opengl::TextureParamWrapT, impl::opengl::TextureWrapRepeat);
	tex->Data_.Texture_.SetParam(impl::opengl::TextureParamMinFilter, impl::opengl::TextureFilterNearest);
	tex->Data_.Texture_.SetParam(impl::opengl::TextureParamMagFilter, impl::opengl::TextureFilterLinear);

	tex->Data_.Texture_.LoadData(impl::opengl::TextureFormatRgba, d.size, fmt, d.data);
	tex->Data_.Texture_.GenMipmaps();

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

void RenderResourceManager::CreateStaticMesh(RStaticMesh *mesh, const std::vector<uint32_t> &indices, const std::vector<uint8_t> &vertexData)
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
