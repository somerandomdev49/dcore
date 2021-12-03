#pragma once
#include <dcore/Data/FileOutput.hpp>
#include <dcore/Data/Json.hpp>

namespace dcore::data::adapters
{
	class JsonOutputAdapter : public OutputAdapter
	{
	public:
		void Write(const std::string &where, const Json &value) override;
	};
} // namespace dcore::data::adapters
