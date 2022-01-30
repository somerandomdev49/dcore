#include <dcore/Util/JsonConverters.hpp>
#include <dcore/Core/UUID.hpp>

namespace dcore::util
{
	data::Json JsonConverters::Glm(const glm::vec2 &v) { return data::Json::array({v.x, v.y}); }

	data::Json JsonConverters::Glm(const glm::vec3 &v) { return data::Json::array({v.x, v.y, v.z}); }

	data::Json JsonConverters::Glm(const glm::vec4 &v) { return data::Json::array({v.x, v.y, v.z, v.w}); }

	data::Json JsonConverters::Glm(const glm::quat &v) { return data::Json::array({v.x, v.y, v.z, v.w}); }

	data::Json JsonConverters::Glm(const glm::mat4 &v)
	{
		return data::Json::array({
		    v[0][0],
		    v[0][1],
		    v[0][2],
		    v[0][3],
		    v[1][0],
		    v[1][1],
		    v[1][2],
		    v[1][3],
		    v[2][0],
		    v[2][1],
		    v[2][2],
		    v[2][3],
		    v[3][0],
		    v[3][1],
		    v[3][2],
		    v[3][3],
		});
	}
} // namespace dcore::util
