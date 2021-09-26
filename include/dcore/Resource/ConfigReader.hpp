#pragma once
#include <dcore/Resource/Resources.hpp>
#include <unordered_map>
#include <string_view>
#include <istream>
#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace dcore::resource
{
	/** Reads all kinds of configuration files */
	class ConfigReader : Resources
	{
	public:
		using DataINI = std::unordered_map<std::string, std::unordered_map<std::string, std::string>>;
		using DataManifest = std::vector<std::string>;

        ConfigReader(const std::string &root);

		/** Reads an INI file */
		bool ReadINI(const std::string &location, DataINI &data);

		/** Reads a manifest file */
		bool ReadManifest(const std::string &location, DataManifest &data);

		glm::ivec2 StringToIVec2(const std::string &s, glm::ivec2 defaultValue = glm::ivec2(0, 0));
		glm::vec2  StringToVec2 (const std::string &s, glm:: vec2 defaultValue = glm:: vec2(0, 0));
		glm::ivec3 StringToIVec3(const std::string &s, glm::ivec3 defaultValue = glm::ivec3(0, 0, 0));
		glm::vec3  StringToVec3 (const std::string &s, glm:: vec3 defaultValue = glm:: vec3(0, 0, 0));
		glm::quat  StringToQuat (const std::string &s, glm:: quat defaultValue = glm:: quat(1, 0, 0, 0));

		static ConfigReader *DefaultReader();
		static void SetDefaultReader(ConfigReader *newDefault);
	private:
		static ConfigReader *DefaultInstance_;
	};
}
