#include <dcore/World/Entity.hpp>
#include <dcore/Util/Debug.hpp>
#include <dcore/Core/Assert.hpp>
#include <typeindex>

namespace dcore::world
{
	ECSComponentPoolProvider *ComponentPoolProviderInstance = nullptr;
	ECSComponentPoolProvider *ECSComponentPoolProvider::Instance()
	{
		if(ComponentPoolProviderInstance == nullptr) ComponentPoolProviderInstance = new ECSComponentPoolProvider();
		return ComponentPoolProviderInstance;
	}

	EntityHandle ECS::Create() { return Registry_.create(); }

	void ECS::Initialize()
	{
		LOG_F(INFO, "ECS::Initialize");
		(void)this;
	}

	void ECS::Broadcast(Message message)
	{
		Registry_.each([message, this](EntityHandle handle) { Send(handle, message); });
	}

	void ECS::DeInitialize() {}

	void ECS::SetMessageHandler(dcore::world::ECS::MessageHandler handler) { MessageHandler_ = handler; }
} // namespace dcore::world
