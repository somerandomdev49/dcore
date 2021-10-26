#include <dcore/Resource/ResourceLoader.hpp>
#include <dcore/Core/Assert.hpp>
#include <dcore/Resource/ConfigReader.hpp>
#include <dcore/Renderer/RStaticMesh.hpp>
#include <dcore/Renderer/RSkeletalMesh.hpp>
#include <dcore/Renderer/RTexture.hpp>
#include <dcore/Renderer/RShader.hpp>
#include <dcore/Renderer/RStaticMesh.hpp>
#include <dcore/Core/Log.hpp>
#include <regex>

using namespace dcore::resource;

ResourceLoader::ResourceLoader(const std::string &root) : Resources(root) {}

void ResourceLoader::LoadFromManifest(const std::string &location)
{
	ConfigReader::DataManifest files;
	ConfigReader::DefaultReader()->ReadManifest(location, files);
	for(const auto &p : files) LoadByName(p);
}

void ResourceLoader::FindMappings_(const std::string &pattern,
                                   std::vector<std::pair<std::string, std::string>> &matched)
{
	static std::string escapes = "[\\^$.|?+(){}";

	// TODO: Optimize this!
	// Escape special characters
	std::string escaped;
	escaped.reserve(pattern.size());
	for(char c : pattern)
	{
		if(escapes.find(c) != escapes.npos) escaped.push_back('\\');
		if(c == '*') // Convert '*' to '.*'
			escaped.push_back('.');
		escaped.push_back(c);
	}

	std::regex rgx(escaped);
	for(auto p : ResMappings_)
	{
		if(std::regex_match(p.first, rgx)) matched.push_back(p);
	}
}

void ResourceLoader::LoadByName(const std::string &name)
{
	// We expect location to be in the form of Type$Path.
	auto splitLoc = name.find('$');
	// TODO: Convert to if
	DCORE_ASSERT(splitLoc != name.npos, "No '$' in resource path");

	auto type = name.substr(0, splitLoc);
	auto path = name.substr(splitLoc + 1);

	DCORE_LOG_INFO << "[ResourceLoader] Loading resource(s) of type " << type << " at " << path;
	std::vector<std::pair<std::string, std::string>> maps;
	FindMappings_(path, maps);
	for(const auto p : maps)
	{
		auto actual = FullPath(p.second);
		DCORE_LOG_INFO << "[ResourceLoader] Loading resource " << p.first << " at " << actual << " [" << type << ']';

		auto found = ResTypes_.find(type);
		DCORE_ASSERT(found != ResTypes_.end(), "Bad Resource Type");
		// LoaderMap_[type](id, actual, res);
		ResourceManager::Instance()->GetRootPath();
		ResourceManager::Instance()->LoadRaw(p.first, p.second, found->second.idx, found->second.allocSize);
	}
}

void ResourceLoader::LoadMappings(const std::string &location)
{
	// auto actual = FullPath(location);
	DCORE_LOG_INFO << "[ResourceLoader] Loading mappings file at " << location;
	ConfigReader::DataINI d;
	int m = ConfigReader::DefaultReader()->ReadINI(location, d);
	DCORE_ASSERT(m, "Could not load ini file");
	ResMappings_ = d["_Default"];
	for(const auto &m : ResMappings_)
	{
		DCORE_LOG_INFO << "> " << m.first << " - " << m.second;
	}
}
