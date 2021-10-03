#include <dcore/Resource/ResourceManager.hpp>

#include <dcore/Core/Assert.hpp>
#include <string_view>
// #include <fwdraw.hpp>
// TODO: Switch to spdlog?
#include <iostream>
using namespace dcore::resource;

RawResource::RawResource() { }
RawResource::RawResource(std::type_index type, void *data) : Type_(type), Data_(data) { }
void *RawResource::Get() const { return Data_; }
const std::type_index &RawResource::GetType() const { return Type_; }

static RawResource NullResource;
static RawResource MissingResource;

// auto impl_ResourceManagerInstance = detail::Impl_ResourceManager();
// detail::Impl_ResourceManager::Impl_ResourceManager()
// {
//     NullResource.Data_ = nullptr;
//     NullResource.Type_ = RT_ERROR;

//     MissingResource.Data_ = nullptr;
//     MissingResource.Type_ = RT_MISSING;
// }

void ResourceManager::Initialize() {}
void ResourceManager::DeInitialize()
{
    DCORE_LOG_INFO << "[ResourceManager] De-Initializing...";

    for(const auto &m : Resources_)
    {
        DCORE_LOG_INFO << "[ResourceManager] Removing [" << m.first.name() << ']';
        for(const auto &p : m.second)
        {
            DeConstructors_[p.second.GetType()](p.second.GetType(), p.first);
        }
    }
}

void ResourceManager::UnLoadRaw(const std::string &id, std::type_index type)
{
    DCORE_LOG_INFO << "[ResourceManager] Removing Resource [" << detail::StringResourceTypeEnum(type) << "] '" << id << '\'';
    DCORE_ASSERT_RETURN(type >= 0 && type < RT_RESOURCE_COUNT, "ResourceManager::RemoveResource: Incorrect Resource Type!");
    if(Resources_[type].find(id) == Resources_[type].end())
    { DCORE_LOG_ERROR << "Tried removing non-existent resource! '" << id << "'"; return; }

    // DCORE_LOG_INFO << "[ResourceManager] Removing Resource: '" << id << "'";

    RawResource &r = Resources_[type][id];

    r.Type_ = RT_MISSING;
    r.Data_ = nullptr;
}

void ResourceManager::AddResource(const std::string &id, const RawResource &res)
{
    DCORE_ASSERT_RETURN(res.GetType() < RT_RESOURCE_COUNT, "ResourceManager::AddResource: Incorrect Resource Type!");
    DCORE_LOG_INFO << "Adding resource of type [" << detail::StringResourceTypeEnum(res.GetType()) << "] '" << id << '\'';
    Resources_[res.GetType()][id] = res;
}

const RawResource &ResourceManager::GetRaw(const std::string &id, ResourceType type)
{
    if(type < 0 || type >= RT_RESOURCE_COUNT)
    {
        // TODO: Add parametrized assert&return macro
        internal_Assert("[ResourceManager::GetRaw] Incorrect Resource Type!", "type < 0 || type >= RT_RESOURCE_COUNT", __FILE__, __LINE__);
        return NullResource;
    }

    if(Resources_[type].find(id) == Resources_[type].end())
    {
        DCORE_LOG_ERROR << "[ResourceManager::GetRaw] Resource '" << id << "\' doesn't exist.";
        return MissingResource;
    }
    
    return Resources_[type][id];
}
