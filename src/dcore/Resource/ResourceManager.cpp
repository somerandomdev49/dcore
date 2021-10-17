#include <dcore/Resource/ResourceManager.hpp>
#include <dcore/Core/Assert.hpp>
#include <dcore/Util/Debug.hpp>
#include <string_view>
#include <iostream>

using namespace dcore::resource;

RawResource::RawResource() {}
RawResource::RawResource(std::type_index type, void *data) : Type_(type), Data_(data) {}
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

ResourceManager *resMngrInstance = nullptr;
ResourceManager *ResourceManager::Instance()
{
	if(!resMngrInstance) resMngrInstance = new ResourceManager("data");
	return resMngrInstance;
}

void ResourceManager::SetInstance(ResourceManager *newInstance) { resMngrInstance = newInstance; }

ResourceManager::ResourceManager(const std::string &root) : Resources(root) {}

void ResourceManager::Initialize() {}
void ResourceManager::DeInitialize()
{
	DCORE_LOG_INFO << "[ResourceManager] De-Initializing...";

	for(auto &m : Resources_)
	{
		DCORE_LOG_INFO << "[ResourceManager] Removing [" << util::Debug::Demangle(m.first.name()) << ']';
		for(auto &p : m.second)
		{
			DeConstructors_[p.second.GetType()](p.second.Data_);
			p.second.Data_ = nullptr;
		}
	}
}

void ResourceManager::UnLoadRaw(const std::string &id, std::type_index type)
{
	DCORE_LOG_INFO << "[ResourceManager] Removing Resource [" << type.name() << "] '" << id << '\'';
	// DCORE_ASSERT_RETURN(type >= 0 && type < RT_RESOURCE_COUNT, "ResourceManager::RemoveResource:
	// Incorrect Resource Type!");
	if(Resources_[type].find(id) == Resources_[type].end())
	{
		DCORE_LOG_ERROR << "Tried removing non-existent resource! '" << id << "'";
		return;
	}

	// DCORE_LOG_INFO << "[ResourceManager] Removing Resource: '" << id << "'";

	RawResource &r = Resources_[type][id];
	DeConstructors_[type](r.Data_);

	r.Type_ = std::type_index(typeid(Null));
	r.Data_ = nullptr;
}

const RawResource &ResourceManager::LoadRaw(const std::string &id, const std::string &location, std::type_index idx, size_t allocSize)
{
	// DCORE_ASSERT_RETURN(res.GetType() < RT_RESOURCE_COUNT, "ResourceManager::AddResource:
	// Incorrect Resource Type!");
	DCORE_LOG_INFO << "Adding resource of type [" << dcore::util::Debug::Demangle(idx.name()) << "] '" << id << '\'';

	void *bytes = new char[allocSize];
	Constructors_[idx](FullPath(location), bytes);
	auto &res = (Resources_[idx][id] = RawResource(idx, bytes));
	return res;
}

const RawResource &ResourceManager::GetRaw(const std::string &id, std::type_index type)
{
	static auto missingResource = RawResource(std::type_index(typeid(Null)), nullptr);
	// if(type < 0 || type >= RT_RESOURCE_COUNT)
	// {
	//     // TODO: Add parametrized assert&return macro
	//     internal_Assert("[ResourceManager::GetRaw] Incorrect Resource Type!", "type < 0 || type
	//     >= RT_RESOURCE_COUNT", __FILE__, __LINE__); return NullResource;
	// }

	if(Resources_[type].find(id) == Resources_[type].end())
	{
		DCORE_LOG_ERROR << "[ResourceManager::GetRaw] Resource '" << id << "\' doesn't exist.";
		return missingResource;
	}

	return Resources_[type][id];
}

void ResourceManager::RegisterConstructor(const std::type_index &type, ResourceConstructorFunc func) { Constructors_[type] = func; }
void ResourceManager::RegisterDeConstructor(const std::type_index &type, ResourceDeConstructorFunc func) { DeConstructors_[type] = func; }
