#pragma once
#include <dcore/Launch.hpp>

namespace dg
{
	class Game
	{
	private:
		friend class ::dcore::launch::Launch;
		void Initialize();
		void DeInitialize();
	};
}
