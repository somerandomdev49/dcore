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
}

void Renderer::OnEndRender()
{
}

void Renderer::Initialize()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::DeInitialize()
{
	// ?
}

void Renderer::Render(RShader *shader, RStaticMesh *mesh, RTexture *texture)
{
	if(shader) glUseProgram(shader->Data_.Id);
	if(texture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTextureUnit(GL_TEXTURE_2D, texture->Data_.Texture_.Id_);
	}

	glBindVertexArray(mesh->Data_.Vao_.VAO_);
	// TODO: Use GL_TRIANGLE_STRIP instead of triangles.
	// TODO: Will make the loading process more complicated, but will decrease memory usage.
	glDrawElements(GL_TRIANGLES, mesh->Data_.Vao_.IndexCount_, GL_UNSIGNED_INT, 0);
}

void Renderer::SetUniform(const RUniform &u, float v)
{
	glUniform1f(u.Data_.Location, v);
}

void Renderer::SetUniform(const RUniform &u, int v)
{
	glUniform1i(u.Data_.Location, v);
}

void Renderer::SetUniform(const RUniform &u, const glm::vec2 &v)
{
	glUniform2f(u.Data_.Location, v.x, v.y);
}

void Renderer::SetUniform(const RUniform &u, const glm::vec3 &v)
{
	glUniform3f(u.Data_.Location, v.x, v.y, v.z);
}

void Renderer::SetUniform(const RUniform &u, const glm::vec4 &v)
{
	glUniform4f(u.Data_.Location, v.x, v.y, v.z, v.w);
}

void Renderer::SetUniform(const RUniform &u, const glm::mat2x2 &v)
{
	glUniformMatrix2fv(u.Data_.Location, 1, GL_FALSE, glm::value_ptr(v));
}

void Renderer::SetUniform(const RUniform &u, const glm::mat2x3 &v)
{
	glUniformMatrix2x3fv(u.Data_.Location, 1, GL_FALSE, glm::value_ptr(v));
}

void Renderer::SetUniform(const RUniform &u, const glm::mat2x4 &v)
{
	glUniformMatrix2x4fv(u.Data_.Location, 1, GL_FALSE, glm::value_ptr(v));
}

void Renderer::SetUniform(const RUniform &u, const glm::mat3x2 &v)
{
	glUniformMatrix3x2fv(u.Data_.Location, 1, GL_FALSE, glm::value_ptr(v));
}

void Renderer::SetUniform(const RUniform &u, const glm::mat3x3 &v)
{
	glUniformMatrix3fv(u.Data_.Location, 1, GL_FALSE, glm::value_ptr(v));
}

void Renderer::SetUniform(const RUniform &u, const glm::mat3x4 &v)
{
	glUniformMatrix3x4fv(u.Data_.Location, 1, GL_FALSE, glm::value_ptr(v));
}

void Renderer::SetUniform(const RUniform &u, const glm::mat4x2 &v)
{
	glUniformMatrix4x2fv(u.Data_.Location, 1, GL_FALSE, glm::value_ptr(v));
}

void Renderer::SetUniform(const RUniform &u, const glm::mat4x3 &v)
{
	glUniformMatrix4x3fv(u.Data_.Location, 1, GL_FALSE, glm::value_ptr(v));
}

void Renderer::SetUniform(const RUniform &u, const glm::mat4x4 &v)
{
	glUniformMatrix4fv(u.Data_.Location, 1, GL_FALSE, glm::value_ptr(v));
}

RUniform Renderer::GetUniform(RShader *shader, const char *name)
{
	RUniform u;
	u.Data_.Location = glGetUniformLocation(shader->Data_.Id, name);
	return u;
}

void Renderer::UseShader(RShader *shader)
{
	glUseProgram(shader->Data_.Id);
}

void Renderer::RTexture_Constructor(const std::string &path, void *placement)
{
	static impl::opengl::TextureFormat formats[4] = {impl::opengl::TextureFormatR, impl::opengl::TextureFormatRg,
	                                                 impl::opengl::TextureFormatRgb, impl::opengl::TextureFormatRgba};

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
}

void Renderer::RStaticMesh_Constructor(const std::string &path, void *placement)
{
	RStaticMesh *mesh = new(placement) RStaticMesh();
	util::MeshData d;
	util::LoaderUtil::LoadMesh(d, path, "pnt");

	mesh->Data_.Vao_.Load(d.indices, d.verticexData, d.stride);
	mesh->Data_.Vao_.CreateFloatAttribute(3); // Position
	mesh->Data_.Vao_.CreateFloatAttribute(3); // Normal
	mesh->Data_.Vao_.CreateFloatAttribute(2); // TexCoord
}

void Renderer::RStaticMesh_DeConstructor(void *placement)
{
	RStaticMesh *mesh = reinterpret_cast<RStaticMesh *>(placement);
	mesh->Data_.Vao_.Delete();
}

void Renderer::RShader_Constructor(const std::string &path, void *placement)
{
	RShader *shader = new(placement) RShader();
}

void Renderer::RShader_DeConstructor(void *placement)
{
	RShader *shader = reinterpret_cast<RShader *>(placement);
}

void RenderResourceManager::Register(resource::ResourceLoader *rl)
{
	rl->RegisterResourceType<RTexture>("Texture");
	resource::ResourceManager::Instance()->RegisterConstructor<RTexture>(&Renderer::RTexture_Constructor);
	resource::ResourceManager::Instance()->RegisterDeConstructor<RTexture>(&Renderer::RTexture_DeConstructor);

	rl->RegisterResourceType<RShader>("Shader");
	resource::ResourceManager::Instance()->RegisterConstructor<RShader>(&Renderer::RShader_Constructor);
	resource::ResourceManager::Instance()->RegisterDeConstructor<RShader>(&Renderer::RShader_DeConstructor);

	rl->RegisterResourceType<RStaticMesh>("Mesh");
	resource::ResourceManager::Instance()->RegisterConstructor<RStaticMesh>(&Renderer::RStaticMesh_Constructor);
	resource::ResourceManager::Instance()->RegisterDeConstructor<RStaticMesh>(&Renderer::RStaticMesh_DeConstructor);
}
