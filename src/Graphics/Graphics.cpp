#include <dcore/Graphics/Shader.hpp>

using namespace dcore::graphics;

Shader::Shader(const resource::Resource<fwdraw::Shader> &sh)
    : Shader_(sh), UTransform_(sh.Get(), "u_Transform") { }

fwdraw::Shader *Shader::Get() const { return Shader_.Get(); }
void Shader::SetTransform(const glm::mat4 &m) { UTransform_.set(m); }
