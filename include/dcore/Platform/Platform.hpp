#pragma once

namespace dcore::platform
{
	class PlatformSpecific
	{
	public:
		void Initialize();
		void Deinitialize();
	};

	class Context
	{
	public:
		void Initialize();
		void OpenWindow();
		void CloseWindow();
		void DeInitialize();
	};
}
