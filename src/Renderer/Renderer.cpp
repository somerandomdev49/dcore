#include <dcore/Renderer/Renderer.hpp>
#include <dcore/Core/Log.hpp>
using namespace dcore::graphics;

// RendererCommand::RendererCommand(fwdraw::Shader *shader, fwdraw::Mesh *mesh)
//     : shader(shader), mesh(mesh)
// {
//     type = RendererCommandType::Normal;
// }

// fwdraw::Shader *RendererCommand::GetRawShader() const
// { return shader; }

// fwdraw::Mesh *RendererCommand::GetRawMesh() const
// { return mesh; }

// RendererCommandType RendererCommand::GetType() const
// { return type; }


// RendererCommandTextured::RendererCommandTextured(fwdraw::Shader *shader, fwdraw::Mesh *mesh, fwdraw::Texture *texture)
//     : RendererCommand(shader, mesh)
// {
//     type = RendererCommandType::Textured;
//     textures.push_back(texture);
// }

// RendererCommandTextured::RendererCommandTextured(fwdraw::Shader *shader, fwdraw::Mesh *mesh, const std::vector<fwdraw::Texture*> &textures)
//     : RendererCommand(shader, mesh)
// {
//     type = RendererCommandType::Textured;
//     this->textures.insert(this->textures.begin(), textures.begin(), textures.end());
// }

// const std::vector<fwdraw::Texture*> &RendererCommandTextured::GetRawTextures() const
// { return textures; }

// void Renderer::Submit(RendererCommand *command)
// {
//     Commands_.push_back(command);
// }

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

void Renderer::Render(fwdraw::Shader *shader, fwdraw::Mesh *mesh, fwdraw::Texture *texture)
{
    if(shader) Renderer_.bind_shader(shader);
    if(!shader) { DCORE_LOG_ERROR << "Shader is null"; }

    // int idx = 0;
    // for(const auto &t : t())
    // {
    if(texture) Renderer_.bind_texture(0, texture);
    // idx += 1;
    // }

    if(mesh) Renderer_.render_mesh(mesh);
}

// void Renderer::FlushQueue()
// {
//     auto prevSize = Commands_.size();
//     while(Commands_.size())
//     {
//         RendererCommand *c = Commands_.front();
//         Commands_.erase(Commands_.begin());
//         if(!c) continue;

//         switch(c->GetType())
//         {
//         case RendererCommandType::Normal:
//         {
//             if(c->GetRawShader()) Renderer_.bind_shader(c->GetRawShader());
//             if(c->GetRawMesh()) Renderer_.render_mesh(c->GetRawMesh());
//         } break;
//         case RendererCommandType::Textured:
//         {
//             RendererCommandTextured *tc = reinterpret_cast<RendererCommandTextured*>(c);
//             if(tc->GetRawShader()) Renderer_.bind_shader(tc->GetRawShader());

//             int idx = 0;
//             for(const auto &t : tc->GetRawTextures())
//             {
//                 if(t) Renderer_.bind_texture(idx, t);
//                 idx += 1;
//             }

//             if(c->GetRawMesh()) Renderer_.render_mesh(c->GetRawMesh());
//         }
//         default: continue;
//         }
//     }
//     Commands_.reserve(prevSize);
// }
