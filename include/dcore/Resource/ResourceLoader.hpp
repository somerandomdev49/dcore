#pragma once
#include <dcore/Resource/Resources.hpp>
#include <dcore/Resource/ResourceManager.hpp>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <string>

namespace dcore::resource
{
    /** Class responsible for loading resources */
    class ResourceLoader : public Resources
    {
    public:
        ResourceLoader(const std::string &root);

        /**
         * \brief Loads resources from a manifest file.
         * \param location The relative path for the manifest file.
         * \param res The ResourceManager to load the resources into.
         **/
        void LoadManifest(const std::string &location, ResourceManager *res);

        
        /**
         * \brief Loads a resource from the specified file.
         * \param location The relative path for the resource file.
         * \param res The ResourceManager to load the resource into.
         **/
        void Load(const std::string &location, ResourceManager *res);

        /**
         * \brief Loads a mapping file from the specified path.
         * \param the path to load the file from.
         **/
        void LoadMappings(const std::string &location);
    private:
        void FindMappings_(const std::string &pattern, std::vector<std::string> &matching);
        void ActualLoad_(
            const std::string &type,
            const std::string &id,
            const std::string &location,
            ResourceManager *res);

        using LoaderFunc = void(*)(const std::string&, const std::string&, ResourceManager*);

        std::unordered_map<std::string, std::string> ResMappings_;
        static std::unordered_map<std::string, LoaderFunc> LoaderMap_;

        static void MeshLoader(const std::string &id, const std::string &location, ResourceManager *res);
        static void TextureLoader(const std::string &id, const std::string &location, ResourceManager *res);
        static void AudioLoader(const std::string &id, const std::string &location, ResourceManager *res);
        static void ShaderLoader(const std::string &id, const std::string &location, ResourceManager *res);
    };
}
