// #define FWDRAW_IMPL
#include <fwdraw.hpp>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <glm/ext.hpp>

namespace internal
{
    // std::string getPathFromFilename(const std::string &str) { return str.substr(0, str.find_last_of("/\\")); }
    char const* errorToString(GLenum const err) noexcept
    {
        switch (err)
        {
        // opengl 2 errors (8)
        case GL_NO_ERROR: return "GL_NO_ERROR";

        case GL_INVALID_ENUM: return "GL_INVALID_ENUM";

        case GL_INVALID_VALUE: return "GL_INVALID_VALUE";

        case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";

        case GL_STACK_OVERFLOW: return "GL_STACK_OVERFLOW";

        case GL_STACK_UNDERFLOW: return "GL_STACK_UNDERFLOW";

        case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";

        // case GL_TABLE_TOO_LARGE:
        // return "GL_TABLE_TOO_LARGE";

        // opengl 3 errors (1)
        case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";

        // gles 2, 3 and gl 4 error are handled by the switch above
        default:
            assert(!"unknown error");
            return nullptr;
        }
    }


    void loadObjModel(const std::string &fp, std::vector<fwdraw::Vertex> &vertices, std::vector<fwdraw::Index> &indices)
    {
        vertices.clear();
        indices.clear();
        tinyobj::ObjReader reader;

        if(!reader.ParseFromFile(fp)) {
            if(!reader.Error().empty()) {
                std::cerr << "[tinyobjloader] error: " << reader.Error();
            }
            std::exit(1);
        }

        if (!reader.Warning().empty()) {
            std::cout << "[tinyobjloader] warning: " << reader.Warning();
        }

        auto& attrib = reader.GetAttrib();
        auto& shapes = reader.GetShapes();
        auto& materials = reader.GetMaterials();

        // std::unordered_map<
        // size_t current_index = 0;

        for(size_t s = 0; s < shapes.size(); s++) {
            
            size_t index_offset = 0;
            // vertices.shrink_to_fit();
            vertices.reserve(vertices.size() + shapes[s].mesh.num_face_vertices.size() * 3);
            for(size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {

                for (size_t v = 0; v < 3; v++) {
                    fwdraw::Vertex vertex;

                    // access to vertex
                    tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                    tinyobj::real_t vx = attrib.vertices[3*size_t(idx.vertex_index)+0];
                    tinyobj::real_t vy = attrib.vertices[3*size_t(idx.vertex_index)+1];
                    tinyobj::real_t vz = attrib.vertices[3*size_t(idx.vertex_index)+2];
                    vertex.position = glm::vec3(vx, vy, vz);

                    // Check if `normal_index` is zero or positive. negative = no normal data
                    if(idx.normal_index >= 0) {
                        tinyobj::real_t nx = attrib.normals[3*size_t(idx.normal_index)+0];
                        tinyobj::real_t ny = attrib.normals[3*size_t(idx.normal_index)+1];
                        tinyobj::real_t nz = attrib.normals[3*size_t(idx.normal_index)+2];
                        vertex.normal = glm::vec3(nx, ny, nz);
                    }

                    // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                    if (idx.texcoord_index >= 0) {
                        tinyobj::real_t tx = attrib.texcoords[3*size_t(idx.texcoord_index)+0];
                        tinyobj::real_t ty = attrib.texcoords[3*size_t(idx.texcoord_index)+1];
                        tinyobj::real_t tz = attrib.texcoords[3*size_t(idx.texcoord_index)+2];
                        vertex.texcoord = glm::vec3(tx, ty, tz);
                    }

                    // Optional: vertex colors
                    // tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
                    // tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
                    // tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
                    vertices.push_back(vertex);
                    indices.push_back(indices.size());

                }
                index_offset += 3;

                // per-face material
                // shapes[s].mesh.material_ids[f];
            }
        }

    }

    // https://stackoverflow.com/a/116220/9110517
    void loadFile(std::string &data, const char *filename)
    {
        constexpr auto read_size = std::size_t{4096};
        std::ifstream stream(filename);
        stream.exceptions(std::ios_base::badbit);

        auto buf = std::string(read_size, '\0');
        while(stream.read(&buf[0], read_size))
            data.append(buf, 0, stream.gcount());
        data.append(buf, 0, stream.gcount());
    }

    void checkGlErrorsDefaultImpl(const char* stmt, const char* fname, int line)
    {
        GLenum err = glGetError();
        if(err != GL_NO_ERROR)
        {
            std::printf("OpenGL error %08x (%s), at %s:%i - for %s\n", err, errorToString(err), fname, line, stmt);
            std::exit(1);
        }
    }

    void (*checkGlErrors)(const char *stmt, const char *fname, int line) = &checkGlErrorsDefaultImpl;
    
    bool shouldLog = true;
    void logGlDefaultImpl(const char *stmt)
    {
        if(shouldLog)
            std::fprintf(stderr, "\033[1;30m[opengl] %s\033[0;0m\n", stmt);
    }

    void (*logGl)(const char *stmt)  = &logGlDefaultImpl;
}


#define _DEBUG
#define DEBUG_GL_LOG

#ifdef _DEBUG
#  ifdef DEBUG_GL_LOG
#    define CGL(stmt) do{internal::logGl(#stmt);stmt;internal::checkGlErrors(#stmt, __FILE__, __LINE__);}while(0)
#    define SVGL(name,stmt) do{internal::logGl(#name " = " #stmt);name=(stmt);internal::checkGlErrors(#stmt, __FILE__, __LINE__);}while(0)
#  else
#    define CGL(stmt) do{stmt;internal::checkGlErrors(#stmt, __FILE__, __LINE__);}while(0)
#    define SVGL(name,stmt) do{name=(stmt);internal::checkGlErrors(#stmt, __FILE__, __LINE__);}while(0)
#  endif
#else
#define GL_CHECK(stmt) stmt
#endif

namespace fwdraw
{
    Frame::Frame(const glm::ivec2 &size)
        : size_(size) { }

    void Frame::init()
    {
        if(!glfwInit()) return;
        glfwSetErrorCallback([](int code, const char *message)
        {
            std::cerr << "[GLFW] Error: " << code << " " << message << std::endl;
        });

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        auto win = glfwCreateWindow(size_.x, size_.y, "FW Draw", NULL, NULL);
        if(win == NULL)
        {
            std::cerr << "[GLFW] Error: " << "Could not create window!" << std::endl;
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(win);

        if(gl3wInit())
        {
            std::cerr << "[GL3W] Error: " << "Could not initialize!" << std::endl;
            glfwDestroyWindow(win);
            glfwTerminate();
            return;
        }

        if(!gl3wIsSupported(4, 1))
        {
            std::cerr << "[GL3W] Error: " << "OpenGL 4.1 is not supported!" << std::endl;
            glfwDestroyWindow(win);
            glfwTerminate();
            return;
        }
        // if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        // {
        //     logError("Failed to initialize GLAD");
        //     return;
        // }

        hndl_ = win;

        CGL(glEnable(GL_DEPTH_TEST));
    }

    void Frame::deinit()
    {
        glfwDestroyWindow((GLFWwindow*)hndl_);
    }

    void Frame::on_begin()
    {
        current_time_ = glfwGetTime();
        delta_time_ = current_time_ - prev_time_;
    }

    void Frame::on_end()
    {
        prev_time_ = current_time_;
        glfwSwapBuffers((GLFWwindow*)hndl_);
        glfwPollEvents();
    }

    bool Frame::key_pressed(int key) const
    { return glfwGetKey((GLFWwindow*)hndl_, key); }
    
    bool Frame::mouse_pressed(int button) const
    { return glfwGetMouseButton((GLFWwindow*)hndl_, button); }

    const glm::ivec2 &Frame::size() const { return size_; }
    const glm::ivec2 &Frame::size(const glm::ivec2 &new_size)
    {
        size_ = new_size;
        glfwSetWindowSize((GLFWwindow*)hndl_, size_.x, size_.y);
        return size_;
    }

    float Frame::delta() const { return delta_time_; }
    bool Frame::should_end() { return glfwWindowShouldClose((GLFWwindow*)hndl_); }

    Mesh::Mesh() {}

    Mesh::Mesh(const char *filename, MeshFileType type)
    {
        switch(type)
        {
            case MODEL_OBJ:
            {
                std::vector<Vertex> vertices;
                std::vector<Index> indices;
                internal::loadObjModel(filename, vertices, indices);
                init(vertices, indices);
            } break;
            default:
            {
                vbo_ = -1; ebo_ = -1; vao_ = -1;
                std::cerr << "Unsupported file format: " << int(type) << std::endl;
            }
        }
    }

    void Mesh::init(const std::vector<Vertex> &vertices, const std::vector<Index> &indices)
    {
        // std::cout << "Initializing mesh with " << vertices.size() << " vertices and " <<
        //     indices.size() << " indices..." << std::endl;

        icount = indices.size();

        CGL(glGenBuffers(1, &vbo_));
        CGL(glGenBuffers(1, &ebo_));
        CGL(glGenVertexArrays(1, &vao_));
        CGL(glBindVertexArray(vao_));
        CGL(glBindBuffer(GL_ARRAY_BUFFER, vbo_));
        CGL(glBufferData(GL_ARRAY_BUFFER,
            vertices.size() * sizeof(Vertex),
            &vertices[0],
            GL_STATIC_DRAW
        ));

        CGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_));
        CGL(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            indices.size() * sizeof(Index),
            &indices[0],
            GL_STATIC_DRAW
        ));

        CGL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position)));
        CGL(glEnableVertexAttribArray(0));

        CGL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal)));
        CGL(glEnableVertexAttribArray(1));

        CGL(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord)));
        CGL(glEnableVertexAttribArray(2));

        CGL(glBindVertexArray(0));
    }

    unsigned int Mesh::index_count() const { return icount; }
    unsigned int Mesh::get_id() const { return vao_; }

    void Mesh::deinit()
    {
        CGL(glDeleteBuffers(1, &vbo_));
        CGL(glDeleteBuffers(1, &ebo_));
        CGL(glDeleteVertexArrays(1, &vao_));
    }

    void Shader::init(const std::string &vert_source, const std::string &frag_source)
    {
        const char *c_vert_source = vert_source.c_str();
        const char *c_frag_source = frag_source.c_str();
        std::cout << "vertex shader:\n" << c_vert_source << std::endl;
        std::cout << "fragment shader:\n" << c_frag_source << std::endl;

        int shader_ok;
        char errors[512];

        GL::UInt vs; SVGL(vs, glCreateShader(GL_VERTEX_SHADER));
        CGL(glShaderSource(vs, 1, &c_vert_source, NULL));
        CGL(glCompileShader(vs));

        CGL(glGetShaderiv(vs, GL_COMPILE_STATUS, &shader_ok));
        if(!shader_ok)
        {
            CGL(glGetShaderInfoLog(vs, 512, NULL, errors));
            std::cerr << "Vertex shader compilation error:\n" << errors << std::endl;
            return;
        }

        GL::UInt fs; SVGL(fs, glCreateShader(GL_FRAGMENT_SHADER));
        CGL(glShaderSource(fs, 1, &c_frag_source, NULL));
        CGL(glCompileShader(fs));

        CGL(glGetShaderiv(fs, GL_COMPILE_STATUS, &shader_ok));
        if(!shader_ok)
        {
            CGL(glGetShaderInfoLog(fs, 512, NULL, errors));
            std::cerr << "Fragment shader compilation error:\n" << errors << std::endl;
            return;
        }

        SVGL(id_, glCreateProgram());
        CGL(glAttachShader(id_, vs));
        CGL(glAttachShader(id_, fs));
        CGL(glLinkProgram(id_));

        CGL(glGetProgramiv(id_, GL_LINK_STATUS, &shader_ok));
        if(!shader_ok)
        {
            CGL(glGetProgramInfoLog(id_, 512, NULL, errors));
            std::cerr << "Linking error:\n" << errors << std::endl;
            return;
        }

        CGL(glDeleteShader(vs));
        CGL(glDeleteShader(fs));
    }

    unsigned int Shader::get_id() const { return id_; }

    void Shader::deinit()
    {
        CGL(glDeleteProgram(id_));
    }

    Shader::Shader(const char *vertex_filename, const char *fragment_filename)
    {
        std::string vert, frag;
        internal::loadFile(vert, vertex_filename);
        internal::loadFile(frag, fragment_filename);
        init(vert, frag);
    }

    Uniform::Uniform(Shader *shader, const char *name)
        : shader_(shader)
    {
        SVGL(id_, glGetUniformLocation(shader->get_id(), name));
    }

    Uniform::Uniform(Shader *shader, GL::Int id)
        : shader_(shader), id_(id) { }

    void Uniform::set(float v) const { CGL(glUniform1f(id_, v)); }
    void Uniform::set(int v) const { CGL(glUniform1i(id_, v)); }
    void Uniform::set(const glm::vec2 &v) const { CGL(glUniform2f(id_, v.x, v.y)); }
    void Uniform::set(const glm::vec3 &v) const { CGL(glUniform3f(id_, v.x, v.y, v.z)); }
    void Uniform::set(const glm::vec4 &v) const { CGL(glUniform4f(id_, v.x, v.y, v.z, v.w)); }
    void Uniform::set(const glm::mat2x2 &v) const { CGL(glUniformMatrix2fv  (id_, 1, GL::False, glm::value_ptr(v))); }
    void Uniform::set(const glm::mat2x3 &v) const { CGL(glUniformMatrix2x3fv(id_, 1, GL::False, glm::value_ptr(v))); }
    void Uniform::set(const glm::mat2x4 &v) const { CGL(glUniformMatrix2x4fv(id_, 1, GL::False, glm::value_ptr(v))); }
    void Uniform::set(const glm::mat3x2 &v) const { CGL(glUniformMatrix3x2fv(id_, 1, GL::False, glm::value_ptr(v))); }
    void Uniform::set(const glm::mat3x3 &v) const { CGL(glUniformMatrix3fv  (id_, 1, GL::False, glm::value_ptr(v))); }
    void Uniform::set(const glm::mat3x4 &v) const { CGL(glUniformMatrix3x4fv(id_, 1, GL::False, glm::value_ptr(v))); }
    void Uniform::set(const glm::mat4x2 &v) const { CGL(glUniformMatrix4x2fv(id_, 1, GL::False, glm::value_ptr(v))); }
    void Uniform::set(const glm::mat4x3 &v) const { CGL(glUniformMatrix4x3fv(id_, 1, GL::False, glm::value_ptr(v))); }
    void Uniform::set(const glm::mat4x4 &v) const { CGL(glUniformMatrix4fv  (id_, 1, GL::False, glm::value_ptr(v))); }

    Texture::Texture() {}
    Texture::Texture(const char *filename)
    {
        // std::puts("what");
        glm::ivec2 size;
        int ccount;
        // std::cout << "Loading image at '" << filename << "'..." << std::endl;
        unsigned char *data = stbi_load(filename, &size.x, &size.y, &ccount, 0);
        if(data) init(data, size, ccount);
        else
        {
            std::cerr << "Failed to load image at '" << filename << '\'';
            if(stbi_failure_reason())
                std::cout << ": " << stbi_failure_reason() << std::endl;
            else
                std::cout << std::endl;
        }
    }

    void Texture::init(void *data, const glm::ivec2 &size, size_t channel_count)
    {
        std::cout << "Initializing texture... " << channel_count << " channels, "
            << size.x << "x" << size.y << std::endl;
        auto fmt = channel_count == 3 ? GL_RGB : GL_RGBA;

        std::printf("%x\n", *(uint32_t*)data);
        std::printf("this = 0x%x\n", this);

        CGL(glGenTextures(1, &id_));
        CGL(glBindTexture(GL_TEXTURE_2D, id_));

        CGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        CGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
        CGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        CGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

        CGL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, fmt, GL_UNSIGNED_BYTE, data));
        CGL(glGenerateMipmap(GL_TEXTURE_2D));

        std::printf("Done Creating Texture\n");
    }

    GL::UInt Texture::get_id() const { return id_; }

    void Texture::deinit() { CGL(glDeleteTextures(1, &id_)); }

    TextureArray::TextureArray() {}
    TextureArray::TextureArray(int count, const char *filenames[])
    {
        std::vector<TextureData> datas; datas.reserve(count);
        for(int i = 0; i < count; ++i)
        {
            glm::ivec2 size;
            int ccount;
            std::cout << "Loading image at '" << filenames[i] << "'..." << std::endl;
            unsigned char *data = stbi_load(filenames[i], &size.x, &size.y, &ccount, 0);

            if(datas.size() != 0 && size != datas[0].size)
            {
                std::cerr << "Image at '" << filenames[i] << '\'' << " doesn't have the same size as the first one: "
                    << size.x << "x" << size.y << " <-> " << datas[0].size.x << "x" << datas[0].size.y << std::endl;
            }

            if(data) datas.push_back(TextureData{ data, size, (size_t)ccount });
            else
            {
                std::cerr << "Failed to load image at '" << filenames[i] << '\'';
                if(stbi_failure_reason())
                    std::cout << ": " << stbi_failure_reason() << std::endl;
                else
                    std::cout << std::endl;
            }
        }
        init(datas);
    }

    void TextureArray::init(const std::vector<TextureData> &datas)
    {
        std::cout << "Initializing texture array of " << datas.size() << " textures..." << std::endl;
        if(datas.size() == 0) return;
        // std::cout << "Initializing texture #" << i << channel_count << " channels, "
        //     << size.x << "x" << size.y << std::endl;

        // std::printf("this = 0x%x\n", this);

        CGL(glGenTextures(1, &id_));
        CGL(glBindTexture(GL_TEXTURE_2D_ARRAY, id_));

        CGL(glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, datas[0].size.x, datas[0].size.y, datas.size()));

        for(int i = 0; i < datas.size(); ++i)
        {
            auto fmt = datas[i].channel_count == 3 ? GL_RGB : GL_RGBA;
            CGL(glTexSubImage3D(
                GL_TEXTURE_2D_ARRAY,
                0,
                0, 0, i,
                datas[i].size.x, datas[i].size.y, 1,
                fmt,
                GL_UNSIGNED_BYTE,
                datas[i].data));
            // CGL(glGenerateMipmap(GL_TEXTURE_2D));
        }

        CGL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT));
        CGL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT));
        CGL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        CGL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    }

    GL::UInt TextureArray::get_id() const { return id_; }

    void TextureArray::deinit() { CGL(glDeleteTextures(1, &id_)); }

    void Renderer::init() {}
    void Renderer::deinit() {}

    void Renderer::bind_shader(Shader *s)
    {
        CGL(glUseProgram(s->get_id()));
    }

    void Renderer::bind_texture(int index, fwdraw::Texture *texture)
    {
        if(!texture) return;
        CGL(glActiveTexture(GL_TEXTURE0 + index));
        CGL(glBindTexture(GL_TEXTURE_2D, texture->get_id()));
    }

    void Renderer::bind_texture(int index, fwdraw::TextureArray *texture)
    {
        if(!texture) return;
        CGL(glActiveTexture(GL_TEXTURE0 + index));
        CGL(glBindTexture(GL_TEXTURE_2D_ARRAY, texture->get_id()));
    }

    void Renderer::render_mesh(Mesh *mesh)
    {
        CGL(glBindVertexArray(mesh->get_id()));
        CGL(glDrawElements(GL_TRIANGLES, mesh->index_count(), GL_UNSIGNED_INT, 0));
    }

    void Renderer::begin()
    {
        internal::shouldLog = false;
        CGL(glClearColor(.0f, .0f, .0f, 1.f));
        CGL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }
}
