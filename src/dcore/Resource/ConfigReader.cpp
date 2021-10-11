#include <dcore/Resource/ConfigReader.hpp>
#include <dcore/Core/Assert.hpp>
#include <dcore/Core/Log.hpp>
#include <fstream>

using namespace dcore::resource;

ConfigReader *ConfigReader::DefaultInstance_ = nullptr;

// TODO: Deallocate this somehow.
ConfigReader *CreateDefaultReader() {
	return new ConfigReader("");
}

ConfigReader::ConfigReader(const std::string &root) : Resources(root) {
}

/** Reads a manifest file */
bool ConfigReader::ReadManifest(const std::string &location, DataManifest &data) {
	std::ifstream ifs(FullPath(location));
	if(!ifs) return false;
	for(std::string line; std::getline(ifs, line);) {
		// trim the string
		auto start = line.find_first_not_of(" \n\t\r\v");
		auto end   = line.find_last_not_of(" \n\t\r\v");
		if(line[start] == '#') continue; // Comment
		if(start == end) continue;       // Empty line.
		data.push_back(line.substr(start, end + 1));
	}

	return true;
}

bool ConfigReader::ReadINI(const std::string &location, DataINI &data) {
	std::string s = FullPath(location);
	std::ifstream ifs(s);
	if(!ifs) {
		DCORE_LOG_ERROR << "Could not load file at '" << s << '\'';
		return false;
	}
	std::string name = "_Default";
	int lineno       = 0;
	for(std::string line; std::getline(ifs, line);) {
		lineno += 1;

		// trim the string
		auto start = line.find_first_not_of(" \n\t\r\v");
		auto end   = line.find_last_not_of(" \n\t\r\v");
		if(line[start] == '#') continue; // Comment
		if(start == end) continue;       // Empty line.

		if(line[start] == '[') // Tag
		{
			// [tag]
			if(line[end] != ']') {
				LOG_F(ERROR, "Expected ']' in %s:%d (column %ld)", s.c_str(), lineno, end);
				return false;
			}
			name = line.substr(start + 1, end);
		} else {
			// key = value
			auto equalsLoc = line.find("=");
			if(equalsLoc == line.npos) {
				LOG_F(ERROR, "No '=' in %s:%d", s.c_str(), lineno);
				return false;
			}
			auto key   = line.substr(start, equalsLoc);
			auto value = line.substr(equalsLoc + 1, end + 1);

			auto keyEnd                           = key.find_last_not_of(" \t\r\n\v");
			auto valueStart                       = value.find_first_not_of(" \t\r\n\v");
			data[name][key.substr(0, keyEnd + 1)] = value.substr(valueStart);
		}
	}

	return true;
}

ConfigReader *ConfigReader::DefaultReader() {
	if(DefaultInstance_ == nullptr) SetDefaultReader(CreateDefaultReader());
	return DefaultInstance_;
}

void ConfigReader::SetDefaultReader(ConfigReader *newDefault) {
	DefaultInstance_ = newDefault;
}
