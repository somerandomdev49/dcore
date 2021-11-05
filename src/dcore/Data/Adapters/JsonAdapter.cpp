#include <dcore/Data/Adapters/JsonAdapter.hpp>
#include <dcore/Core/Log.hpp>
#include <fstream>

using namespace dcore::data::adapters;

void JsonOutputAdapter::Write(const std::string &path, const picojson::value &value)
{
	std::ofstream out(path);
	if(!out)
	{
		DCORE_LOG_ERROR << "[JsonOutputAdapter] Failed to open file at '" << path << "'!";
		return;
	}

	value.serialize(std::ostream_iterator<char>(out));

	out.close();
}
