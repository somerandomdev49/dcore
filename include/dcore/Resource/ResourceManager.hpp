#pragma once
#include <dcore/Uni.hpp>
#include <unordered_map>
#include <string_view>
#include <cinttypes>
#include <array>
#include <fwdraw.hpp>

namespace dcore::resource
{
	enum ResourceType : char
	{
		RT_ERROR = -2,
		RT_MISSING = -1,
		RT_STATIC_MESH = 0,
		RT_SKELETAL_MESH,
		RT_TEXTURE_2D,
		RT_TEXTURE_3D,
		RT_TEXTURE_CUBEMAP,
		RT_SHADER,
		RT_RESOURCE_COUNT // keep this last.
		// also, keep the enum values auto-generated,
		// ResourceManager needs the values to be
		// in a sequence.
	};
	
	namespace detail
	{
		/**
		 * Converts a type to a ResourceType enum value.
		 * Example: `EnumResourceType\<fwdraw::Mesh>() -> RT_STATIC_MESH`
		**/
		template<typename T>
		constexpr ResourceType EnumResourceType();

		template<>
		constexpr ResourceType EnumResourceType<fwdraw::Mesh>()
		{ return RT_STATIC_MESH; }
		
		template<>
		constexpr ResourceType EnumResourceType<fwdraw::Shader>()
		{ return RT_SHADER; }

		template<>
		constexpr ResourceType EnumResourceType<fwdraw::Texture>()
		{ return RT_TEXTURE_2D; }

#define O(X) case X: return #X
		constexpr const char *StringResourceTypeEnum(ResourceType t)
		{
			switch(t)
			{
				O(RT_ERROR);
				O(RT_MISSING);
				O(RT_STATIC_MESH);
				O(RT_SKELETAL_MESH);
				O(RT_TEXTURE_2D);
				O(RT_TEXTURE_3D);
				O(RT_TEXTURE_CUBEMAP);
				O(RT_SHADER);
				O(RT_RESOURCE_COUNT);
				default: return "<BAD_RESOURCE_TYPE>";
			}
		}
#undef O

		class Impl_ResourceManager { public: Impl_ResourceManager(); };
	}

	/** A wrapper around void* */
	class RawResource
	{
	public:
		RawResource();

		void *Get() const;
		ResourceType GetType() const;
	private:
		friend class ResourceLoader;
		friend class ResourceManager;
		friend class detail::Impl_ResourceManager;
		RawResource(ResourceType type, void *data);
		ResourceType Type_ = RT_ERROR;
		void DCORE_REF *Data_ = nullptr;
	};

	/** A wrapper around some kind of data. */
	template<typename T>
	class Resource
	{
	public:
		Resource() : Data_(nullptr) {};

		T *Get() const { return Data_; };
	private:
		friend class ResourceManager;
		Resource(T *data) : Data_(data) {}
		T DCORE_REF *Data_;
	};


	// TODO: Add Lazy loading of resources...?
	/** Manages all of the resources of the game. */
	class ResourceManager
	{
	public:
		void Initialize();

		/** Returns a raw resource for a type with a specified id. */
		const RawResource &GetRaw(const std::string &id, ResourceType type);

		/** Returns a resource of a specified type and id. */
		template<typename T>
		Resource<T> Get(const std::string &id);

		void DeInitialize();
	private:
		friend class ResourceLoader;
		using ResourceMap = std::unordered_map<std::string, RawResource>;

		void AddResource(const std::string &id, const RawResource &res);
		void RemoveResource(ResourceType type, const std::string &id);

		std::array<ResourceMap, RT_RESOURCE_COUNT> Resources_;
	};

	template<typename T>
	Resource<T> ResourceManager::Get(const std::string &id)
	{ return Resource<T>(reinterpret_cast<T*>(GetRaw(id, detail::EnumResourceType<T>()).Data_)); }
}
