#include <dcore/Resource/ResourceManager.hpp>

#include <dcore/Core/Assert.hpp>
#include <string_view>
#include <fwdraw.hpp>
// TODO: Switch to spdlog?
#include <iostream>
using namespace dcore::resource;

RawResource::RawResource() { }
RawResource::RawResource(ResourceType type, void *data) : Type_(type), Data_(data) { }
void *RawResource::Get() const { return Data_; }
ResourceType RawResource::GetType() const { return Type_; }

static RawResource NullResource;
static RawResource MissingResource;

auto impl_ResourceManagerInstance = detail::Impl_ResourceManager();
detail::Impl_ResourceManager::Impl_ResourceManager()
{
    NullResource.Data_ = nullptr;
    NullResource.Type_ = RT_ERROR;

    MissingResource.Data_ = nullptr;
    MissingResource.Type_ = RT_MISSING;
}

void ResourceManager::Initialize() {}
void ResourceManager::DeInitialize()
{
    DCORE_LOG_INFO << "[ResourceManager] De-Initializing...";

    int i = 0;
    for(const auto &m : Resources_)
    {
        DCORE_LOG_INFO << "[ResourceManager] Removing [" << detail::StringResourceTypeEnum(ResourceType(i)) << ']';
        for(const auto &p : m)
        {
            RemoveResource(p.second.GetType(), p.first);
        }
        i += 1;
    }
}

void ResourceManager::RemoveResource(ResourceType type, const std::string &id)
{
    DCORE_LOG_INFO << "[ResourceManager] Removing Resource [" << detail::StringResourceTypeEnum(type) << "] '" << id << '\'';
    DCORE_ASSERT_RETURN(type >= 0 && type < RT_RESOURCE_COUNT, "ResourceManager::RemoveResource: Incorrect Resource Type!");
    if(Resources_[type].find(id) == Resources_[type].end())
    { DCORE_LOG_ERROR << "Tried removing non-existent resource! '" << id << "'"; return; }

    // DCORE_LOG_INFO << "[ResourceManager] Removing Resource: '" << id << "'";

    RawResource &r = Resources_[type][id];

    switch(type)
    {
    case RT_STATIC_MESH: {
        auto a = reinterpret_cast<fwdraw::Mesh*>(r.Get());
        if(!a) break;
        a->deinit(); delete a;
    } break;
    case RT_SHADER: {
        auto a = reinterpret_cast<fwdraw::Shader*>(r.Get());
        if(!a) break;
        a->deinit(); delete a;
    } break;
    case RT_TEXTURE_2D: {
        auto a = reinterpret_cast<fwdraw::Texture*>(r.Get());
        if(!a) break;
        a->deinit(); delete a;
    } break;
    case RT_ERROR: break;
    case RT_MISSING: break;
    default: break;
    }

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
