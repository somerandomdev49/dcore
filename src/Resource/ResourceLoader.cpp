#include <dcore/Resource/ResourceLoader.hpp>
#include <dcore/Core/Assert.hpp>
#include <dcore/Resource/ConfigReader.hpp>
#include <iostream>
#include <fwdraw.hpp>
#include <regex>

using namespace dcore::resource;

void ResourceLoader::MeshLoader(const std::string_view &id, const std::string_view &location, ResourceManager *res)
{
    res->AddResource(id, RawResource(RT_STATIC_MESH, (void*)(new fwdraw::Mesh(std::string(location).c_str()))));
}

void ResourceLoader::TextureLoader(const std::string_view &id, const std::string_view &location, ResourceManager *res)
{
    res->AddResource(id, RawResource(RT_TEXTURE_2D, (void*)(new fwdraw::Texture(std::string(location).c_str()))));
}

void ResourceLoader::AudioLoader(const std::string_view &id, const std::string_view &location, ResourceManager *res)
{
    (void)location;
    // TODO
    res->AddResource(id, RawResource(RT_ERROR, nullptr));
}
void ResourceLoader::ShaderLoader(const std::string_view &id, const std::string_view &location, ResourceManager *res)
{
    auto vs = std::string(location) + ".vert";
    auto fs = std::string(location) + ".frag";
    res->AddResource(id, RawResource(RT_SHADER, (void*)(new fwdraw::Shader(vs.c_str(), fs.c_str()))));
}

std::unordered_map<std::string_view, ResourceLoader::LoaderFunc> ResourceLoader::LoaderMap_ =
{
    {"Mesh", &ResourceLoader::MeshLoader},
    {"Texture", &ResourceLoader::TextureLoader},
    {"Audio", &ResourceLoader::AudioLoader},
    {"Shader", &ResourceLoader::ShaderLoader},
};

ResourceLoader::ResourceLoader(const std::string_view &root)
    : Resources(root) {}

void ResourceLoader::LoadManifest(const std::string_view &location, ResourceManager *res)
{
    ConfigReader::DataManifest files;
    ConfigReader::DefaultReader()->ReadManifest(location, files);
    for(const auto &p : files) Load(p, res);
}

void ResourceLoader::FindMappings_(const std::string_view &pattern, std::vector<std::string> &matched)
{
    static std::string_view escapes = "[\\^$.|?+(){}";

    // TODO: Optimize this!
    // Escape special characters
    std::string escaped; escaped.reserve(pattern.size());
    for(char c : pattern)
    {
        if(escapes.find(c) != escapes.npos)
            escaped.push_back('\\');
        if(c == '*') // Convert '*' to '.*'
            escaped.push_back('.');
        escaped.push_back(c);
    }

    std::regex rgx(escaped);
    for(auto p : ResMappings_)
    {
        if(std::regex_match(p.first, rgx))
            matched.push_back(p.second);
    }
}

void ResourceLoader::Load(const std::string_view &location, ResourceManager *res)
{
    // We expect location to be in the form of Type$Path.
    auto splitLoc = location.find('$');
    DCORE_ASSERT(splitLoc != location.npos, "No '$' in resource path");

    auto type = location.substr(0, splitLoc);
    auto path = location.substr(splitLoc + 1);

    std::cout << "Loading resource of type " << type << " at " << path << std::endl;
    std::vector<std::string> maps;
    FindMappings_(path, maps);
    for(const auto p : maps)
        ActualLoad_(type, path, p, res);
}

void ResourceLoader::ActualLoad_(
    const std::string_view &type, const std::string_view &id, const std::string_view &location, ResourceManager *res)
{
    auto actual = FullPath(location);
    std::cout << "Loading resource " << id << " at " << actual << " [" << type << "]" << std::endl;
    DCORE_ASSERT(LoaderMap_.find(type) != LoaderMap_.end(), "Bad Resource Type");
    LoaderMap_[type](id, actual, res);
}
