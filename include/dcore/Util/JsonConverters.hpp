#pragma once
#include <dcore/Data/Json.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace dcore::util
{
	class JsonConverters
	{
	public:
		static data::Json Glm(const glm::vec2 &value);
		static data::Json Glm(const glm::vec3 &value);
		static data::Json Glm(const glm::vec4 &value);
		static data::Json Glm(const glm::quat &value);
		static data::Json Glm(const glm::mat4 &value);
	};
} // namespace dcore::util
