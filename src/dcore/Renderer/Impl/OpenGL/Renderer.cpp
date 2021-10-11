#include <dcore/Renderer/Impl/OpenGL.hpp>
#include <dcore/Renderer/Renderer.hpp>
#include <dcore/Core/Log.hpp>
#include <dcore/Util/LoaderUtil.hpp>
#include <dcore/Resource/ResourceLoader.hpp>
#include <glm/ext.hpp>
using namespace dcore::graphics;

void Renderer::OnBeginRender()
{
    
}

void Renderer::OnEndRender() {}

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
        glBindTextureUnit(GL_TEXTURE_2D, texture->Data_.Id);
    }

    glBindVertexArray(mesh->Data_.Vao_.VAO);
    // TODO: Use GL_TRIANGLE_STRIP instead of triangles.
    // TODO: Will make the loading process more complicated, but will decrease memory usage.
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Renderer::SetUniform(const RUniform &u, float v)
{ glUniform1f(u.Data_.Location, v); }

void Renderer::SetUniform(const RUniform &u, int v)
{ glUniform1i(u.Data_.Location, v); }

void Renderer::SetUniform(const RUniform &u, const glm::vec2 &v)
{ glUniform2f(u.Data_.Location, v.x, v.y); }

void Renderer::SetUniform(const RUniform &u, const glm::vec3 &v)
{ glUniform3f(u.Data_.Location, v.x, v.y, v.z); }

void Renderer::SetUniform(const RUniform &u, const glm::vec4 &v)
{ glUniform4f(u.Data_.Location, v.x, v.y, v.z, v.w); }

void Renderer::SetUniform(const RUniform &u, const glm::mat2x2 &v)
{ glUniformMatrix2fv(u.Data_.Location, 1, GL_FALSE, glm::value_ptr(v)); }

void Renderer::SetUniform(const RUniform &u, const glm::mat2x3 &v)
{ glUniformMatrix2x3fv(u.Data_.Location, 1, GL_FALSE, glm::value_ptr(v)); } 

void Renderer::SetUniform(const RUniform &u, const glm::mat2x4 &v)
{ glUniformMatrix2x4fv(u.Data_.Location, 1, GL_FALSE, glm::value_ptr(v)); }

void Renderer::SetUniform(const RUniform &u, const glm::mat3x2 &v)
{ glUniformMatrix3x2fv(u.Data_.Location, 1, GL_FALSE, glm::value_ptr(v)); }

void Renderer::SetUniform(const RUniform &u, const glm::mat3x3 &v)
{ glUniformMatrix3fv(u.Data_.Location, 1, GL_FALSE, glm::value_ptr(v)); }

void Renderer::SetUniform(const RUniform &u, const glm::mat3x4 &v)
{ glUniformMatrix3x4fv(u.Data_.Location, 1, GL_FALSE, glm::value_ptr(v)); }

void Renderer::SetUniform(const RUniform &u, const glm::mat4x2 &v)
{ glUniformMatrix4x2fv(u.Data_.Location, 1, GL_FALSE, glm::value_ptr(v)); }

void Renderer::SetUniform(const RUniform &u, const glm::mat4x3 &v)
{ glUniformMatrix4x3fv(u.Data_.Location, 1, GL_FALSE, glm::value_ptr(v)); }

void Renderer::SetUniform(const RUniform &u, const glm::mat4x4 &v)
{ glUniformMatrix4fv(u.Data_.Location, 1, GL_FALSE, glm::value_ptr(v)); }

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
    static GLenum formats[4] = { GL_RED, GL_RG, GL_RGB, GL_RGBA };
	RTexture *tex = new(placement) RTexture(); // This constructs a RTexture at the specified address (see "placement new")

    util::ImageData d = util::LoaderUtil::LoadImage(path);
    auto fmt = formats[d.channels - 1];

    glGenTextures(1, &tex->Data_.Id);
    glBindTexture(GL_TEXTURE_2D, tex->Data_.Id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, d.size.x, d.size.y, 0, fmt, GL_UNSIGNED_BYTE, d.data);
    glGenerateMipmap(GL_TEXTURE_2D);

    free(d.data);
}

void Renderer::RTexture_DeConstructor(void *placement)
{
    RTexture *tex = reinterpret_cast<RTexture*>(placement);
    glDeleteTextures(1, &tex->Data_.Id);
}

void Renderer::RStaticMesh_Constructor(const std::string &path, void *placement)
{
	RStaticMesh *mesh = new(placement) RStaticMesh();
    util::MeshData d = util::LoaderUtil::LoadMesh(path);
    
    mesh->Data_.Vao_.Create(d.indices, d.verticexData);
}

void Renderer::RStaticMesh_DeConstructor(void *placement)
{    
    RStaticMesh *mesh = reinterpret_cast<RStaticMesh*>(placement);

}

void Renderer::RShader_Constructor(const std::string &path, void *placement)
{
	RShader *shader = new(placement) RShader();

}

void Renderer::RShader_DeConstructor(void *placement)
{
    RShader *shader = reinterpret_cast<RShader*>(placement);
    
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

