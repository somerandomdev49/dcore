#include <dcore/Resource/ResourceManager.hpp>
using namespace dcore::resource;

#include <dcore/Core/Assert.hpp>
#include <string_view>

void *RawResource::Get() const { return Data_; }
ResourceType RawResource::GetType() const { return Type_; }

void ResourceManager::AddResource(const std::string_view &id, const RawResource &res)
{
    DCORE_ASSERT(res.GetType() < RT_RESOURCE_COUNT, "ResourceManager::AddResource: Incorrect Resource Type!");
    Resources_[res.GetType()][id] = res;
}

RawResource ResourceManager::GetRaw(const std::string_view &id, ResourceType type)
{
    DCORE_ASSERT(type < RT_RESOURCE_COUNT, "ResourceManager::GetRaw: Incorrect Resource Type!");
    return Resources_[type][id];
}
