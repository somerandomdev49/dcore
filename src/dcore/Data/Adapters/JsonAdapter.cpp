#include <dcore/Data/Adapters/JsonAdapter.hpp>
#include <dcore/Core/Log.hpp>
#include <fstream>
#include <iostream>

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

	Json JsonInputAdapter::Read(const std::string &path)
	{
		std::ifstream inp(path);
		if(!inp)
		{
			DCORE_LOG_ERROR << "[JsonInputAdapter] Failed to open file at '" << path << "'!";
			return Json {};
		}
		Json j;
		inp >> j;
		inp.close();
		return j;
	}
} // namespace dcore::data::adapters
