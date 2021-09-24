#include <dcore/Resource/ResourceManager.hpp>
using namespace dcore::resource;

#include <dcore/Core/Assert.hpp>
#include <string_view>
#include <fwdraw.hpp>
// TODO: Switch to spdlog
#include <iostream>

RawResource::RawResource() { }
RawResource::RawResource(ResourceType type, void *data) : Type_(type), Data_(data) { }
void *RawResource::Get() const { return Data_; }
ResourceType RawResource::GetType() const { return Type_; }

void ResourceManager::Initialize() {}
void ResourceManager::DeInitialize()
{
    DCORE_LOG_INFO << "[ResourceManager] De-Initializing...";

    int i = 0;
    for(const auto &m : Resources_)
    {
        DCORE_LOG_INFO << "[ResourceManager] Removing [" << detail::StringResourceTypeEnum(ResourceType(i)) << ']';
        for(const auto &p : m)
            RemoveResource(p.second.GetType(), p.first);
        i += 1;
    }
}

void ResourceManager::RemoveResource(ResourceType type, const std::string &id)
{
    DCORE_ASSERT(type < RT_RESOURCE_COUNT, "ResourceManager::RemoveResource: Incorrect Resource Type!");
    if(Resources_[type].find(id) == Resources_[type].end())
    { DCORE_LOG_ERROR << "Tried removing non-existent resource! '" << id << "'"; return; }

    DCORE_LOG_INFO << "[ResourceManager] Removing Resource: '" << id << "'";

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
    DCORE_ASSERT(res.GetType() < RT_RESOURCE_COUNT, "ResourceManager::AddResource: Incorrect Resource Type!");
    DCORE_LOG_INFO << "Adding resource of type [" << detail::StringResourceTypeEnum(res.GetType()) << "] '" << id << '\'';
    Resources_[res.GetType()][id] = res;
}

const RawResource &ResourceManager::GetRaw(const std::string &id, ResourceType type)
{
    DCORE_ASSERT(type < RT_RESOURCE_COUNT, "ResourceManager::GetRaw: Incorrect Resource Type!");
    return Resources_[type][id];
}
