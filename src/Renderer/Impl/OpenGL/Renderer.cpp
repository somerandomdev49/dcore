#include <dcore/Renderer/Impl/OpenGL.hpp>
#include <dcore/Renderer/Renderer.hpp>
#include <dcore/Core/Log.hpp>
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

