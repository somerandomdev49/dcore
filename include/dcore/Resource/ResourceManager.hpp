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
		void *Get() const;
		ResourceType GetType() const;
	private:
		friend class ResourceLoader;
		friend class ResourceManager;
		RawResource(ResourceType *type, void *data);
		ResourceType Type_ = RT_ERROR;
		void *Data_ = nullptr;
	};

	/** A wrapper around some kind of data. */
	template<typename T>
	class Resource
	{
	public:
		T *Get() const { return Data_; };
	private:
		friend class ResourceManager;
		Resource(T *data = nullptr) : Data_(data) {}
		T *Data_;
	};

	/** Manages all of the resources of the game. */
	class ResourceManager
	{
	public:
		RawResource GetRaw(const std::string_view &id, ResourceType type);
	private:
		friend class ResourceLoader;
		using ResourceMap = std::unordered_map<std::string_view, RawResource>;

		void AddResource(const std::string_view &id, const RawResource &res);

		std::array<ResourceMap, RT_RESOURCE_COUNT> Resources_;
	};
}
