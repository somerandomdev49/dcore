#include "renderer.hpp"

RendererCommand::RendererCommand(fwdraw::Shader *shader, fwdraw::Mesh *mesh)
    : shader(shader), mesh(mesh)
{
    type = RendererCommandType::Normal;
}

fwdraw::Shader *RendererCommand::GetShader() const
{ return shader; }

fwdraw::Mesh *RendererCommand::GetMesh() const
{ return mesh; }

RendererCommandType RendererCommand::GetType() const
{ return type; }


RendererCommandTextured::RendererCommandTextured(fwdraw::Shader *shader, fwdraw::Mesh *mesh, fwdraw::Texture *texture)
    : RendererCommand(shader, mesh)
{
    type = RendererCommandType::Textured;
    textures.push_back(texture);
}

RendererCommandTextured::RendererCommandTextured(fwdraw::Shader *shader, fwdraw::Mesh *mesh, const std::vector<fwdraw::Texture*> &textures)
    : RendererCommand(shader, mesh)
{
    type = RendererCommandType::Textured;
    this->textures.insert(this->textures.begin(), textures.begin(), textures.end());
}

const std::vector<fwdraw::Texture*> &RendererCommandTextured::GetTextures() const
{ return textures; }


void Renderer::Init()
{ renderer.init(); }

void Renderer::BeginFrame()
{ renderer.begin(); }

void Renderer::Add(RendererCommand *command)
{ commands.push_back(command); }

void Renderer::Flush()
{
    auto prevSize = commands.size();
    while(commands.size())
    {
        RendererCommand *c = commands.front();
        commands.erase(commands.begin());
        if(!c) continue;

        switch(c->GetType())
        {
        case RendererCommandType::Normal:
        {
            renderer.bind_shader(c->GetShader());
            renderer.render_mesh(c->GetMesh());
        }
        case RendererCommandType::Textured:
        {
            RendererCommandTextured *tc = reinterpret_cast<RendererCommandTextured*>(c);
            renderer.bind_shader(tc->GetShader());

            int idx = 0;
            for(const auto &t : tc->GetTextures())
            {
                renderer.bind_texture(idx, t);
                idx += 1;
            }

            renderer.render_mesh(c->GetMesh());
        }
        default: continue;
        }
    }
    commands.reserve(prevSize);
}

void Renderer::Deinit()
{ renderer.deinit(); }
