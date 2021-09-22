#pragma once
#include <unordered_map>
#include <string_view>
#include <cinttypes>
#include <array>

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
		RawResource(ResourceType type, void *data);
		ResourceType Type_ = RT_ERROR;
		void *Data_ = nullptr;
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
		T *Data_;
	};

	/** Manages all of the resources of the game. */
	class ResourceManager
	{
	public:
		void Initialize();
		RawResource GetRaw(const std::string_view &id, ResourceType type);
		void DeInitialize();
	private:
		friend class ResourceLoader;
		using ResourceMap = std::unordered_map<std::string_view, RawResource>;

		void AddResource(const std::string_view &id, const RawResource &res);
		void RemoveResource(ResourceType type, const std::string_view &id);

		std::array<ResourceMap, RT_RESOURCE_COUNT> Resources_;
	};
}
