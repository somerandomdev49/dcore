#include <dcore/Resource/ResourceLoader.hpp>
#include <dcore/Core/Assert.hpp>
#include <dcore/Resource/ConfigReader.hpp>
#include <dcore/Core/Log.hpp>
#include <fwdraw.hpp>
#include <regex>

using namespace dcore::resource;

void ResourceLoader::RegisterLoader(const std::string &name, LoaderFunc loaderFunc)
{
    LoaderMap_[name] = loaderFunc;
}

void ResourceLoader::MeshLoader(const std::string &id, const std::string &location, ResourceManager *res)
{
    res->AddResource(id, RawResource(RT_STATIC_MESH, (void*)(new fwdraw::Mesh(std::string(location).c_str()))));
}

void ResourceLoader::TextureLoader(const std::string &id, const std::string &location, ResourceManager *res)
{
    res->AddResource(id, RawResource(RT_TEXTURE_2D, (void*)(new fwdraw::Texture(std::string(location).c_str()))));
}

void ResourceLoader::AudioLoader(const std::string &id, const std::string &location, ResourceManager *res)
{
    (void)location;
    // TODO
    res->AddResource(id, RawResource(RT_ERROR, nullptr));
}
void ResourceLoader::ShaderLoader(const std::string &id, const std::string &location, ResourceManager *res)
{
    auto vs = std::string(location) + ".vert";
    auto fs = std::string(location) + ".frag";
    res->AddResource(id, RawResource(RT_SHADER, (void*)(new fwdraw::Shader(vs.c_str(), fs.c_str()))));
}

// std::unordered_map<std::string, ResourceLoader::LoaderFunc> ResourceLoader::LoaderMap_ =
// {
//     {"Mesh", &ResourceLoader::MeshLoader},
//     {"Texture", &ResourceLoader::TextureLoader},
//     {"Audio", &ResourceLoader::AudioLoader},
//     {"Shader", &ResourceLoader::ShaderLoader},
// };

ResourceLoader::ResourceLoader(const std::string &root)
    : Resources(root)
{
    // Default Loaders:
    RegisterLoader("Mesh"   , &ResourceLoader::MeshLoader    );    
    RegisterLoader("Texture", &ResourceLoader::TextureLoader );    
    RegisterLoader("Audio"  , &ResourceLoader::AudioLoader   );    
    RegisterLoader("Shader" , &ResourceLoader::ShaderLoader  );    
}

void ResourceLoader::LoadManifest(const std::string &location, ResourceManager *res)
{
    ConfigReader::DataManifest files;
    ConfigReader::DefaultReader()->ReadManifest(location, files);
    for(const auto &p : files) Load(p, res);
}

void ResourceLoader::FindMappings_(const std::string &pattern, std::vector<std::pair<std::string, std::string>> &matched)
{
    static std::string escapes = "[\\^$.|?+(){}";

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
            matched.push_back(p);
    }
}

void ResourceLoader::Load(const std::string &location, ResourceManager *res)
{
    // We expect location to be in the form of Type$Path.
    auto splitLoc = location.find('$');
    // TODO: Convert to if
    DCORE_ASSERT(splitLoc != location.npos, "No '$' in resource path");

    auto type = location.substr(0, splitLoc);
    auto path = location.substr(splitLoc + 1);

    DCORE_LOG_INFO << "[ResourceLoader] Loading resource of type " << type << " at " << path;
    std::vector<std::pair<std::string, std::string>> maps;
    FindMappings_(path, maps);
    for(const auto p : maps)
        ActualLoad_(type, p.first, p.second, res);
}

void ResourceLoader::ActualLoad_(
    const std::string &type, const std::string &id, const std::string &location, ResourceManager *res)
{
    auto actual = FullPath(location);
    DCORE_LOG_INFO << "[ResourceLoader] Loading resource " << id << " at " << actual << " [" << type << ']';
    DCORE_ASSERT(LoaderMap_.find(type) != LoaderMap_.end(), "Bad Resource Type");
    LoaderMap_[type](id, actual, res);
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
