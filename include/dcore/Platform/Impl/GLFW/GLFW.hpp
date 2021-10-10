#pragma once

namespace dcore::platform::impl::glfw
{
	extern bool DidInitialize;
	extern bool DidTerminate;

	void Initialize();
	void DeInitilize();
	const char *GetError();
}
