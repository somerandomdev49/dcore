#pragma once
#include <dcore/Resource/ResourceManager.hpp>
#include <dcore/Renderer/RShader.hpp>
#include <glm/glm.hpp>
// #include <fwdraw.hpp>

namespace dcore::graphics
{
	class CommonShader
	{
	public:
		CommonShader(const resource::Resource<RShader> &sh);

		RShader *Get() const;
		void SetTransform(const glm::mat4 &m);

	private:
		resource::Resource<RShader> Shader_;
		RUniform UTransform_;
		RUniform UTex_;
	};

	class TerrainShader
	{
	public:
		TerrainShader(const resource::Resource<RShader> &sh);
		RShader *Get() cont;
	}
} // namespace dcore::graphics
