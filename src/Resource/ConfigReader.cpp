#include <dcore/Resource/ConfigReader.hpp>
#include <dcore/Core/Assert.hpp>
#include <fstream>

using namespace dcore::resource;

ConfigReader *ConfigReader::DefaultInstance_ = nullptr;

// TODO: Deallocate this somehow.
ConfigReader *CreateDefaultReader() { return new ConfigReader(""); }

ConfigReader::ConfigReader(const std::string_view &root)
    : Resources(root) {}

/** Reads a manifest file */
bool ConfigReader::ReadManifest(const std::string_view &location, DataManifest &data)
{
    std::ifstream ifs(FullPath(location));
    if(!ifs) return false;
    for(std::string line; std::getline(ifs, line);)
    {
        // trim the string
        auto start = line.find_first_not_of(" \n\t\r\v");
        auto end = line.find_last_not_of(" \n\t\r\v");
        if(line[start] == '#') continue; // Comment
        if(start == end) continue; // Empty line.
        data.push_back(line.substr(start, end + 1));
    }

    return true;
}

ConfigReader *ConfigReader::DefaultReader()
{
    if(DefaultInstance_ == nullptr) SetDefaultReader(CreateDefaultReader());
    return DefaultInstance_;
}

void ConfigReader::SetDefaultReader(ConfigReader *newDefault)
{
    DefaultInstance_ = newDefault;
}
