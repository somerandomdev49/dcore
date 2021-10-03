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
	// enum ResourceType : char
	// {
	// 	RT_ERROR = -2,
	// 	RT_MISSING = -1,
	// 	RT_STATIC_MESH = 0,
	// 	RT_SKELETAL_MESH,
	// 	RT_TEXTURE_2D,
	// 	RT_TEXTURE_3D,
	// 	RT_TEXTURE_CUBEMAP,
	// 	RT_SHADER,
	// 	RT_RESOURCE_COUNT // keep this last.
	// 	// also, keep the enum values auto-generated,
	// 	// ResourceManager needs the values to be
	// 	// in a sequence.
	// };
	
	// namespace detail
	// {
	// 	/**
	// 	 * Converts a type to a ResourceType enum value.
	// 	 * Example: `EnumResourceType\<fwdraw::Mesh>() -> RT_STATIC_MESH`
	// 	**/
	// 	template<typename T>
	// 	constexpr ResourceType EnumResourceType();

	// 	template<>
	// 	constexpr ResourceType EnumResourceType<dcore::graphics::RStaticMesh>()
	// 	{ return RT_STATIC_MESH; }
		
	// 	template<>
	// 	constexpr ResourceType EnumResourceType<dcore::graphics::RShader>()
	// 	{ return RT_SHADER; }

	// 	template<>
	// 	constexpr ResourceType EnumResourceType<dcore::graphics::RTexture>()
	// 	{ return RT_TEXTURE_2D; }

// #define O(X) case X: return #X
// 		constexpr const char *StringResourceTypeEnum(ResourceType t)
// 		{
// 			switch(t)
// 			{
// 				O(RT_ERROR);
// 				O(RT_MISSING);
// 				O(RT_STATIC_MESH);
// 				O(RT_SKELETAL_MESH);
// 				O(RT_TEXTURE_2D);
// 				O(RT_TEXTURE_3D);
// 				O(RT_TEXTURE_CUBEMAP);
// 				O(RT_SHADER);
// 				O(RT_RESOURCE_COUNT);
// 				default: return "<BAD_RESOURCE_TYPE>";
// 			}
// 		}
// #undef O

// 		class Impl_ResourceManager { public: Impl_ResourceManager(); };
// 	} // namespace detail

	/** A wrapper around void* */
	class RawResource
	{
	public:
		RawResource();

		void *Get() const;
		const std::type_info &GetType() const;
	private:
		friend class ResourceLoader;
		friend class ResourceManager;
		// friend class detail::Impl_ResourceManager;
		RawResource(std::type_info type, void *data);
		std::type_info Type_ = RT_ERROR;
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
		ResourceManager *Instance() const;

		/** Returns a raw resource for a type with a specified id. */
		const RawResource &GetRaw(const std::string &id, std::type_info type);

		/** Returns a resource of a specified type and id. */
		template<typename T>
		Resource<T> Get(const std::string &id);

		/** Load a resource with its loader */
		template<typename T>
		Resource<T> Load(const std::string &location);

		/** Unload a resource with its unloader */
		template<typename T>
		Resource<T> UnLoad(const std::string &location);

	private:
		friend class dcore::launch::Launch;
		void Initialize();
		void DeInitialize();
		void SetInstance(ResourceManager *newInstance);

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
	Resource<T> Load(const std::string &location)
	{
		return Resource<T>(reinterpret_cast<T*>(
			LoadRaw(location, std::type_index(typeid(std::decay_t<T>))).Data_
		));
	}

	template<typename T>
	Resource<T> UnLoad(const std::string &location)
	{
		return Resource<T>(reinterpret_cast<T*>(
			UnLoadRaw(location, std::type_index(typeid(std::decay_t<T>))).Data_
		));
	}
}
