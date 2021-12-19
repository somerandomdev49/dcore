#pragma once
#include <dcore/Data/FileOutput.hpp>
#include <dcore/Data/FileInput.hpp>
#include <dcore/Data/Json.hpp>

namespace dcore::data::adapters
{
	class JsonOutputAdapter : public OutputAdapter
	{
	public:
		void Write(const std::string &where, const Json &value) override;
	};

	class JsonInputAdapter : public InputAdapter
	{
	public:
		Json Read(const std::string &where) override;
	};
} // namespace dcore::data::adapters
