#pragma once
#define JSON_HAS_CPP_14
#include <nlohmann/json.hpp>

namespace dcore::data
{
	namespace nlh = nlohmann;
	using Json    = nlh::json;
} // namespace dcore::data
