#pragma once
#include <dcore/Launch.hpp>

namespace dcore::event
{
	class TimeManager
	{
	public:
		void  Initialize();
		void  DeInitialize();
		float GetDeltaTime();

		/** Do not call if not needed. */
		void SetDeltaTime(float newDeltaTime);

		static TimeManager *Instance();

	private:
		friend class launch::Launch;
		static void SetInstance(TimeManager *newInstance);
		float       DeltaTime_;
	};
} // namespace dcore::event
