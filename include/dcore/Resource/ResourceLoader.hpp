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
        ResourceLoader(const std::string_view &root);

        /**
         * \brief Loads resources from a manifest file.
         * \param location The relative path for the manifest file.
         * \param res The ResourceManager to load the resources into.
         **/
        void LoadManifest(const std::string_view &location, ResourceManager *res);

        
        /**
         * \brief Loads a resource from the specified file.
         * \param location The relative path for the resource file.
         * \param res The ResourceManager to load the resource into.
         **/
        void Load(const std::string_view &location, ResourceManager *res);

        void LoadMappings(const std::string_view &location);
    private:
        void FindMappings_(const std::string_view &pattern, std::vector<std::string> &matching);
        void ActualLoad_(
            const std::string_view &type,
            const std::string_view &id,
            const std::string_view &location,
            ResourceManager *res);

        using LoaderFunc = void(*)(const std::string_view&, const std::string_view&, ResourceManager*);

        std::unordered_map<std::string, std::string> ResMappings_;
        static std::unordered_map<std::string_view, LoaderFunc> LoaderMap_;

        static void MeshLoader(const std::string_view &id, const std::string_view &location, ResourceManager *res);
        static void TextureLoader(const std::string_view &id, const std::string_view &location, ResourceManager *res);
        static void AudioLoader(const std::string_view &id, const std::string_view &location, ResourceManager *res);
        static void ShaderLoader(const std::string_view &id, const std::string_view &location, ResourceManager *res);
    };
}
