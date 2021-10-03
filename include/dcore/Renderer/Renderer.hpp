#pragma once
#include <dcore/Renderer/RSkeletalMesh.hpp>
#include <dcore/Renderer/RStaticMesh.hpp>
#include <dcore/Renderer/RTexture.hpp>
#include <dcore/Renderer/RShader.hpp>

namespace dcore::platform { class Context; }

namespace dcore::graphics
{
    /** Direct interface with OpenGL/DX/Vulkan/... */
    class Renderer
    {
    public:
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
    private:
        friend class dcore::platform::Context;
        friend class RenderResourceManager;

        void OnBeginRender();
        // void FlushQueue();
        void OnEndRender();

        void Initialize();
        void DeInitialize();
    };

    /** Class responsible for initializing/deinitializing render resources */
    class RenderResourceManager
    {
    public:
        static 
    };
}
