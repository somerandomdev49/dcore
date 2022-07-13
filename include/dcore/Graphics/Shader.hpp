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

		void SetFogColor(glm::vec3 color);
		void SetTransform(const glm::mat4 &m);

	private:
		resource::Resource<RShader> Shader_;
		RUniform UFogColor_;
		RUniform UTransform_;
		RUniform UTex_;
	};

	class TerrainShader
	{
	public:
		TerrainShader(const resource::Resource<RShader> &sh);
		RShader *Get() const;

		void SetFogColor(glm::vec3 color);
		void SetTransform(const glm::mat4 &m);
		void SetTextures(int blendmap = -1, int none = -1, int r = -1, int g = -1, int b = -1);

	private:
		resource::Resource<RShader> Shader_;
		RUniform UTransform_;
		RUniform UBlendMapTex_;
		RUniform UFogColor_;
		RUniform UTexs_[4]; // 0, R, G, B
	};
} // namespace dcore::graphics
