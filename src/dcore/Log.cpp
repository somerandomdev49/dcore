#include <dcore/Core/Log.hpp>

namespace dstd
{
	void Log::Initialize()
	{
		loguru::add_callback("dstd::Log", [](void *self, const loguru::Message &message)
		{
			((Log*)self)->Messages_.Add(message.message);
		}, this, loguru::NamedVerbosity::Verbosity_INFO);
	}

	void Log::DeInitialize()
	{
		
	}

	static Log *ilog_instance;
	Log *ILog() { return ilog_instance; }
	Log *ILog(Log *newInstance) { return ilog_instance = newInstance; }
}
