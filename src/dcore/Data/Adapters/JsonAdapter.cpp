#include <dcore/Data/Adapters/JsonAdapter.hpp>
#include <dcore/Core/Log.hpp>
#include <fstream>

namespace dcore::data::adapters
{
	void JsonOutputAdapter::Write(const std::string &path, const Json &value)
	{
		std::ofstream out(path);
		if(!out)
		{
			DCORE_LOG_ERROR << "[JsonOutputAdapter] Failed to open file at '" << path << "'!";
			return;
		}

		out << value.dump(); //(std::ostream_iterator<char>(out));

		out.close();
	}
} // namespace dcore::data::adapters
