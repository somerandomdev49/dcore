#pragma once
#include <dcore/Data/FileOutput.hpp>
// #include <dcore/Data/FileInput.hpp>

namespace dcore::data::adapters
{
	class JsonOutputAdapter : public OutputAdapter
	{
	public:
		void Write(const std::string &where, const picojson::value &value) override;
	};
}
