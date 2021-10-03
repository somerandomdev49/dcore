#pragma once
#include <dcore/Resource/ResourceManager.hpp>
#include <dcore/Renderer/RShader.hpp>
#include <glm/glm.hpp>
// #include <fwdraw.hpp>

namespace dcore::graphics
{
    class Shader
    {
    public:
        Shader(const resource::Resource<RShader> &sh);

        RShader *Get() const;
        void SetTransform(const glm::mat4 &m);
    private:
        resource::Resource<RShader> Shader_;
        RUniform UTransform_;
    };
}
