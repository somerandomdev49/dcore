#pragma once
#include <queue>
#include <memory>
#include <glm/glm.hpp>
#include <fwdraw.hpp>

enum class RendererCommandType
{
    Normal, Textured, ArrayTextured
};

class RendererCommand
{
protected:
    RendererCommandType type;
    fwdraw::Shader *shader;
    fwdraw::Mesh *mesh;
public:
    /**
     * Creates a renderer command with a shader and a mesh.
     * if `shader` is nullptr, the previous shader is used.
    */
    RendererCommand(fwdraw::Shader *shader, fwdraw::Mesh *mesh);

    fwdraw::Shader *GetShader() const;
    fwdraw::Mesh *GetMesh() const;
    RendererCommandType GetType() const;
};

class RendererCommandTextured : public RendererCommand
{
protected:
    std::vector<fwdraw::Texture*> textures;
public:
    /**
     * Creates a textured renderer command with a shader, a mesh and a single texture.
     * if `shader` is nullptr, the previous shader is used.
    */
    RendererCommandTextured(fwdraw::Shader *shader, fwdraw::Mesh *mesh, fwdraw::Texture *texture);

    /**
     * Creates a textured renderer command with a shader, a mesh and textures.
     * if `shader` is nullptr, the previous shader is used.
    */
    RendererCommandTextured(fwdraw::Shader *shader, fwdraw::Mesh *mesh, const std::vector<fwdraw::Texture*> &textures);


    const std::vector<fwdraw::Texture*> &GetTextures() const;
};

class RendererCommandArrayTextured : public RendererCommand
{
protected:
    fwdraw::TextureArray *texture;
public:
    /**
     * Creates a texture array renderer command with a shader and a mesh.
     * if `shader` is nullptr, the previous shader is used.
    */
    RendererCommandArrayTextured(fwdraw::Shader *shader, fwdraw::Mesh *mesh, fwdraw::TextureArray *texture);

    fwdraw::TextureArray *GetTextureArray() const;
};

class Renderer
{
public:
    /** Initializes the renderer. */
    void Init();

    /** Should be called at the beginning of each frame. */
    void BeginFrame();

    /** Adds a command to the queue. All commands are deleted after flush. */
    void Add(RendererCommand *command);

    /** Flushes the renderer command queue. */
    void Flush();

    /** Deinitializes the renderer. */
    void Deinit();
private:
    std::vector<RendererCommand*> commands;
    fwdraw::Renderer renderer;
};
