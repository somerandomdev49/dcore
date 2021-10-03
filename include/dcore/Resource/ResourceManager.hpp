#pragma once
#include <dcore/Uni.hpp>
#include <unordered_map>
#include <string>
#include <cinttypes>
#include <array>
#include <typeindex>
#include <typeinfo>
// #include <dcore/Renderer/RShader.hpp>
// #include <dcore/Renderer/RSkeletalMesh.hpp>
// #include <dcore/Renderer/RStaticMesh.hpp>
// #include <dcore/Renderer/RTexture.hpp>
#include <dcore/Resource/Resources.hpp>
#include <dcore/Launch.hpp>

namespace dcore::resource
{
	/** A wrapper around void* */
	class RawResource
	{
	public:
		RawResource();

		void *Get() const;
		const std::type_index &GetType() const;
	private:
		friend class ResourceLoader;
		friend class ResourceManager;
		// friend class detail::Impl_ResourceManager;
		RawResource(std::type_index type, void *data);
		std::type_index Type_ = RT_ERROR;
		void DCORE_REF *Data_ = nullptr;
	};

	/** A wrapper around some kind of data. */
	template<typename T>
	class Resource
	{
	public:
		Resource() : Data_(nullptr) {};

		const T *Get() const { return Data_; };
	private:
		friend class ResourceManager;
		Resource(T *data) : Data_(data) {}
		const T DCORE_REF *Data_;
	};


	// TODO: Add Lazy loading?
	/** Manages all of the resources of the game. */
	class ResourceManager : Resources
	{
	public:
		ResourceManager(const std::string &root);
		static ResourceManager *Instance();

		/** Returns a raw resource for a type with a specified id. */
		const RawResource &GetRaw(const std::string &id, std::type_info type);

		/** Returns a raw resource for a type with a specified id. */
		const RawResource &LoadRaw(const std::string &id, const std::string &location, std::type_index type);

		/** Returns a raw resource for a type with a specified id. */
		const RawResource &UnLoadRaw(const std::string &id, std::type_index type);

		/** Returns a resource of a specified type and id. */
		template<typename T>
		Resource<T> Get(const std::string &id);

		/** Load a resource with its loader */
		template<typename T>
		Resource<T> Load(const std::string &id, const std::string &location);

		/** Unload a resource with its unloader */
		template<typename T>
		Resource<T> UnLoad(const std::string &id);

	private:
		friend class dcore::launch::Launch;
		void Initialize();
		void DeInitialize();
		static void SetInstance(ResourceManager *newInstance);

		using ResourceMap = std::unordered_map<std::string, RawResource>;
		std::unordered_map<std::type_index, ResourceMap> Resources_;

		using ResourceConstructorFunc = void (*)(...);
		std::unordered_map<std::type_index, ResourceConstructorFunc> Constructors_;
		
		using ResourceDeConstructorFunc = void (*)(...);
		std::unordered_map<std::type_index, ResourceDeConstructorFunc> DeConstructors_;
	};

	template<typename T>
	Resource<T> ResourceManager::Get(const std::string &id)
	{
		return Resource<T>(reinterpret_cast<T*>(
			GetRaw(id, std::type_index(typeid(std::decay_t<T>))).Data_
		));
	}

	template<typename T>
	Resource<T> ResourceManager::Load(const std::string &id, const std::string &location)
	{
		return Resource<T>(reinterpret_cast<T*>(
			LoadRaw(id, location, std::type_index(typeid(std::decay_t<T>))).Data_
		));
	}

	template<typename T>
	Resource<T> ResourceManager::UnLoad(const std::string &id)
	{
		return Resource<T>(reinterpret_cast<T*>(
			UnLoadRaw(id, std::type_index(typeid(std::decay_t<T>))).Data_
		));
	}
}
