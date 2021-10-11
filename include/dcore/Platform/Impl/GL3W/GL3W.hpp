#pragma once

namespace dcore::platform::impl::gl3w
{
	void Initialize();
	void DeInitialize();

	extern bool DidInitialize;
	extern bool DidDeInitialize;
}
