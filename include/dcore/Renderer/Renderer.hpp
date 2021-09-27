#pragma once
// #include <dcore/Graphics/StaticMesh.hpp>
#include <dcore/Platform/Platform.hpp>
#include <fwdraw.hpp>

namespace dcore::graphics
{

    // TODO: Review render queues later. (We could do sorting with entt's sort)
    // enum class RendererCommandType
    // {
    //     Normal, Textured, ArrayTextured
    // };

    // class RendererCommand
    // {
    // protected:
    //     RendererCommandType type;
    //     fwdraw::Shader *shader;
    //     fwdraw::Mesh *mesh;
    // public:
    //     /**
    //     /* Creates a renderer command with a shader and a mesh.
    //     /* if `shader` is nullptr, the previous shader is used.
    //     */
    //     RendererCommand(fwdraw::Shader *shader, fwdraw::Mesh *mesh);

    //     fwdraw::Shader *GetRawShader() const;
    //     fwdraw::Mesh *GetRawMesh() const;
    //     RendererCommandType GetType() const;
    // };

    // class RendererCommandTextured : public RendererCommand
    // {
    // protected:
    //     std::vector<fwdraw::Texture*> textures;
    // public:
    //     /**
    //     /* Creates a textured renderer command with a shader, a mesh and a single texture.
    //     /* if `shader` is nullptr, the previous shader is used.
    //     */
    //     RendererCommandTextured(fwdraw::Shader *shader, fwdraw::Mesh *mesh, fwdraw::Texture *texture);

    //     /**
    //     /* Creates a textured renderer command with a shader, a mesh and textures.
    //     /* if `shader` is nullptr, the previous shader is used.
    //     */
    //     RendererCommandTextured(fwdraw::Shader *shader, fwdraw::Mesh *mesh, const std::vector<fwdraw::Texture*> &textures);


    //     const std::vector<fwdraw::Texture*> &GetRawTextures() const;
    // };

    // class RendererCommandArrayTextured : public RendererCommand
    // {
    // protected:
    //     fwdraw::TextureArray *texture;
    // public:
    //     /**
    //     /* Creates a texture array renderer command with a shader and a mesh.
    //     /* if `shader` is nullptr, the previous shader is used.
    //     */
    //     RendererCommandArrayTextured(fwdraw::Shader *shader, fwdraw::Mesh *mesh, fwdraw::TextureArray *texture);

    //     fwdraw::TextureArray *GetRawTextureArray() const;
    // };

    /** Direct interface with fwdraw. */
    class Renderer
    {
    public:
        /**
         * @brief Renders a mesh with a specified texture
         * @note if shader is nullptr, the previous shader is used.
         * @warning Should be called only between OnBeginRender (private) and OnEndRender (private)
        */
        void Render(fwdraw::Shader *shader, fwdraw::Mesh *mesh, fwdraw::Texture *texture);
        // void Submit(RendererCommand *command);
    private:
        friend class dcore::platform::Context;

        void OnBeginRender();
        // void FlushQueue();
        void OnEndRender();

        void Initialize();
        void DeInitialize();

        // std::vector<RendererCommand*> Commands_;
        fwdraw::Renderer Renderer_;
    };
}
