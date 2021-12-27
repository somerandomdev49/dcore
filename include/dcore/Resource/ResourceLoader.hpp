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
		 **/
		void LoadFromManifest(const std::string &location);

		/**
		 * \brief Loads a resource by name (using the mapping)
		 **/
		void LoadByName(const std::string &name);

		/**
		 * \brief Loads a mapping file from the specified path.
		 * \param the path to load the file from.
		 **/
		void LoadMappings(const std::string &location);

		template<typename T>
		void RegisterResourceType(const std::string &name);

	private:
		struct TypeInfo
		{
			std::type_index idx;
			size_t allocSize;
		};

		void FindMappings_(const std::string &pattern,
		                   std::vector<std::pair<std::string, std::string>> &matching);

		std::unordered_map<std::string, std::string> ResMappings_;
		std::unordered_map<std::string, TypeInfo> ResTypes_;
	};

	template<typename T>
	void ResourceLoader::RegisterResourceType(const std::string &name)
	{
		ResTypes_.insert({name, TypeInfo {std::type_index(typeid(std::decay_t<T>)), sizeof(T)}});
	}
} // namespace dcore::resource
