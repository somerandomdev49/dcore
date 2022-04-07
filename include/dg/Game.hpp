#pragma once
#include <dcore/Launch.hpp>
#include <dcore/World/WorldLoader.hpp>

namespace dg
{
	class Game
	{
	private:
		friend class ::dcore::launch::Launch;
		void Initialize();
		void DeInitialize();
		dcore::world::WorldLoaderRegistry Registry_;
	};
} // namespace dg
