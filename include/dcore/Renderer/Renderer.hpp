#pragma once
#include <dcore/Renderer/RSkeletalMesh.hpp>
#include <dcore/Renderer/RStaticMesh.hpp>
#include <dcore/Renderer/RTexture.hpp>
#include <dcore/Renderer/RShader.hpp>
#include <dcore/Resource/ResourceManager.hpp>

namespace dcore::platform { class Context; }

namespace dcore::graphics
{
    /** Direct interface with OpenGL/DX/Vulkan/... */
    class Renderer
    {
    public:
        /**
         * @brief Binds the shader
         * @note All uniforms should be set after this call.
         */
        void UseShader(RShader *shader);

        RUniform GetUniform(RShader *shader, const char *name);
        void SetUniform(const RUniform &u, float v);
        void SetUniform(const RUniform &u, int v);
        void SetUniform(const RUniform &u, const glm::vec2 &v);
        void SetUniform(const RUniform &u, const glm::vec3 &v);
        void SetUniform(const RUniform &u, const glm::vec4 &v);
        void SetUniform(const RUniform &u, const glm::mat2x2 &v);
        void SetUniform(const RUniform &u, const glm::mat2x3 &v);
        void SetUniform(const RUniform &u, const glm::mat2x4 &v);
        void SetUniform(const RUniform &u, const glm::mat3x2 &v);
        void SetUniform(const RUniform &u, const glm::mat3x3 &v);
        void SetUniform(const RUniform &u, const glm::mat3x4 &v);
        void SetUniform(const RUniform &u, const glm::mat4x2 &v);
        void SetUniform(const RUniform &u, const glm::mat4x3 &v);
        void SetUniform(const RUniform &u, const glm::mat4x4 &v);
        
        /**
         * @brief Renders a mesh with a specified texture
         * @note if shader is nullptr, the previous shader is used.
         * @warning Should be called only between OnBeginRender (private) and OnEndRender (private)
        */
        void Render(RShader *shader, RStaticMesh *mesh, RTexture *texture);

        /**
         * @brief Renders a skeletal mesh with a specified texture
         * @note if shader is nullptr, the previous shader is used.
         * @warning Should be called only between OnBeginRender (private) and OnEndRender (private)
        */
        void Render(RShader *shader, RSkeletalMesh *mesh, RTexture *texture);

        static Renderer *Instance();
    private:
        friend class dcore::platform::Context;
        friend class RenderResourceManager;

        static void SetInstance(Renderer *newInstance);

        void OnBeginRender();
        void OnEndRender();

        void Initialize();
        void DeInitialize();
    };

    /** Class responsible for initializing/deinitializing render resources */
    class RenderResourceManager
    {
    public:
        static void Register(resource::ResourceManager *rm);
    };
}
