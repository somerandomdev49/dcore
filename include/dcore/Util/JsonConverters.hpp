#pragma once
#include <dcore/Data/Json.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace dcore::util
{
	class JsonConverters
	{
	public:
		static data::Json Glm(const glm::vec2 &v);
		static data::Json Glm(const glm::vec3 &v);
		static data::Json Glm(const glm::vec4 &v);
		static data::Json Glm(const glm::quat &v);
		static data::Json Glm(const glm::mat4 &v);
	};
} // namespace dcore::util
