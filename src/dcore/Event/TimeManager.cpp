#include <dcore/Event/TimeManager.hpp>
#include <dcore/Platform/Platform.hpp>

namespace dcore::event
{
	void TimeManager::Initialize() { DeltaTime_ = 0; }
	void TimeManager::DeInitialize() {}
	float TimeManager::GetDeltaTime() { return DeltaTime_; }
	void TimeManager::SetDeltaTime(float newDeltaTime) { DeltaTime_ = newDeltaTime; }

	static TimeManager *timMngr;
	TimeManager *TimeManager::Instance()
	{
		if(timMngr == nullptr) timMngr = new TimeManager();
		return timMngr;
	}

	void TimeManager::SetInstance(TimeManager *newInstance) { timMngr = newInstance; }
} // namespace dcore::event
