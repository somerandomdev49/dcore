#pragma once
#include <dcore/Resource/ResourceManager.hpp>
#include <glm/glm.hpp>
#include <fwdraw.hpp>

namespace dcore::graphics
{
    class Shader
    {
    public:
        Shader(const resource::Resource<fwdraw::Shader> &sh);

        fwdraw::Shader *Get() const;
        void SetTransform(const glm::mat4 &m);
    private:
        resource::Resource<fwdraw::Shader> Shader_;
        fwdraw::Uniform UTransform_;
    };
}
