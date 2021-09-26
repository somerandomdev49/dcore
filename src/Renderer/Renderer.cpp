#include <dcore/Renderer/Renderer.hpp>
using namespace dcore::graphics;

RendererCommand::RendererCommand(fwdraw::Shader *shader, fwdraw::Mesh *mesh)
    : shader(shader), mesh(mesh)
{
    type = RendererCommandType::Normal;
}

fwdraw::Shader *RendererCommand::GetRawShader() const
{ return shader; }

fwdraw::Mesh *RendererCommand::GetRawMesh() const
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

const std::vector<fwdraw::Texture*> &RendererCommandTextured::GetRawTextures() const
{ return textures; }

void Renderer::Submit(RendererCommand *command)
{
    Commands_.push_back(command);
}

void Renderer::OnBeginRender()
{
    Renderer_.begin();
}

void Renderer::OnEndRender() {}

void Renderer::Initialize()
{
    Renderer_.init();
}

void Renderer::DeInitialize()
{
    Renderer_.deinit();
}

void Renderer::FlushQueue()
{
    auto prevSize = Commands_.size();
    while(Commands_.size())
    {
        RendererCommand *c = Commands_.front();
        Commands_.erase(Commands_.begin());
        if(!c) continue;

        switch(c->GetType())
        {
        case RendererCommandType::Normal:
        {
            Renderer_.bind_shader(c->GetRawShader());
            Renderer_.render_mesh(c->GetRawMesh());
        } break;
        case RendererCommandType::Textured:
        {
            RendererCommandTextured *tc = reinterpret_cast<RendererCommandTextured*>(c);
            Renderer_.bind_shader(tc->GetRawShader());

            int idx = 0;
            for(const auto &t : tc->GetRawTextures())
            {
                Renderer_.bind_texture(idx, t);
                idx += 1;
            }

            Renderer_.render_mesh(c->GetRawMesh());
        }
        default: continue;
        }
    }
    Commands_.reserve(prevSize);
}
