#pragma once
#define LOGURU_THREADNAME_WIDTH 5
#define LOGURU_WITH_STREAMS     1
#include <loguru.hpp>

#define DCORE_LOG_INFO     LOG_S(INFO)
#define DCORE_LOG_DEBUG    LOG_S(DEBUG)
#define DCORE_LOG_ERROR    LOG_S(ERROR)
#define DCORE_LOG_CRITICAL LOG_S(CRITICAL)
#define DCORE_LOG_WARNING  LOG_S(WARNING)

// TODO: Clean Up this include!
#include <dcore/Core/CircularBuffer.hpp>

namespace dstd
{
	class Log
	{
	public:
		Log(dstd::USize messageBufferSize) : Messages_(messageBufferSize) {}
		void Initialize();
		void DeInitialize();

		const CircularBuffer<std::string> &GetMessages() const { return Messages_; };

	private:
		CircularBuffer<std::string> Messages_;
	};

	Log *ILog();
	Log *ILog(Log *newInstance);
} // namespace dstd
