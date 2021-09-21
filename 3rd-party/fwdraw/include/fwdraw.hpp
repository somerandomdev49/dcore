#ifndef _FWDRAW_HPP
#define _FWDRAW_HPP
#include <vector>
#include <cinttypes>
#include <string>
#include <cmath>
#include <glm/glm.hpp>

extern "C"
{
#include <GL/gl3w.h>
}

namespace fwdraw
{
    class Frame
    {
    public:
        Frame(const glm::ivec2 &size);
        void init();
        void deinit();
        void on_begin();
        void on_end();
        bool should_end();

        bool key_pressed(int key) const;
        bool mouse_pressed(int key) const;

        float delta() const;

        const glm::ivec2 &size() const;
        const glm::ivec2 &size(const glm::ivec2 &new_size);
    private:
        glm::ivec2 size_;
        void *hndl_;
        double current_time_, prev_time_;
        float delta_time_;
    };

    namespace GL
    {
        using UInt = GLuint;
        using Int = GLint;
        constexpr auto False = GL_FALSE;
        constexpr auto True = GL_FALSE;
    }

    struct Vertex { glm::vec3 position; glm::vec3 normal; glm::vec3 texcoord; };
    using Index = std::uint32_t;

    class Texture
    {
    public:
        Texture(const char *filepath);
        Texture();
        void init(void *data, const glm::ivec2 &size, size_t channel_count = 4);
        void deinit();

        GL::UInt get_id() const;
    private:
        GL::UInt id_;
    };

    struct TextureData { void *data; glm::ivec2 size; size_t channel_count; };

    class TextureArray
    {
    public:
        TextureArray(int count, const char *filenames[]);
        TextureArray();
        void init(const std::vector<TextureData> &datas);
        void deinit();

        GL::UInt get_id() const;
    private:
        GL::UInt id_;
    };

    class Shader;
    class Uniform
    {
    public:
        Uniform(Shader *shader, const char *name);
        Uniform(Shader *shader, GL::Int id);

        void set(float v) const;
        void set(int v) const;
        void set(const glm::vec2 &v) const;
        void set(const glm::vec3 &v) const;
        void set(const glm::vec4 &v) const;
        void set(const glm::mat2x2 &v) const;
        void set(const glm::mat2x3 &v) const;
        void set(const glm::mat2x4 &v) const;
        void set(const glm::mat3x2 &v) const;
        void set(const glm::mat3x3 &v) const;
        void set(const glm::mat3x4 &v) const;
        void set(const glm::mat4x2 &v) const;
        void set(const glm::mat4x3 &v) const;
        void set(const glm::mat4x4 &v) const;
    private:
        Shader *shader_;
        GL::Int id_;
    };

    class Shader
    {
    public:
        Shader(const char *vertex_filename, const char *fragment_filename);
        Shader();
        void init(const std::string &vertex_source, const std::string &fragment_source);
        void deinit();

        GL::UInt get_id() const;
    private:
        GL::UInt id_;
    };

    class Mesh {
    public:
        enum MeshFileType { MODEL_OBJ, MODEL_GLTF };
        Mesh();
        Mesh(const char *filepath, MeshFileType type = MODEL_OBJ);
        void init(const std::vector<Vertex> &vertices, const std::vector<Index> &indices);
        void deinit();

        unsigned int index_count() const;
        GL::UInt get_id() const;
    private:
        GL::UInt vbo_, ebo_, vao_;
        unsigned int icount;
    };

    class Renderer
    {
    public:
        void init();
        void deinit();
        void begin();
        void bind_texture(int index, Texture *texture);
        void bind_texture(int index, TextureArray *texture);
        void bind_shader(Shader *shader);
        void render_mesh(Mesh *mesh);
    };
}

#endif//_FWDRAW_HPP
