#pragma once
#include <dcore/Resource/Resources.hpp>
#include <unordered_map>
#include <string_view>
#include <istream>
#include <string>
#include <vector>

namespace dcore::resource
{
	/** Reads all kinds of configuration files */
	class ConfigReader : Resources
	{
	public:
		using DataINI = std::unordered_map<std::string, std::string>;
		using DataManifest = std::vector<std::string>;

        ConfigReader(const std::string_view &root);

		/** Reads an INI file */
		bool ReadINI(const std::string_view &location, DataINI &data);

		/** Reads a manifest file */
		bool ReadManifest(const std::string_view &location, DataManifest &data);

		static ConfigReader *DefaultReader();
		static void SetDefaultReader(ConfigReader *newDefault);
	private:
		static ConfigReader *DefaultInstance_;
	};
}
