#pragma once
#define LOGURU_THREADNAME_WIDTH 5
#define LOGURU_WITH_STREAMS     1
#include <loguru.hpp>

#define DCORE_LOG_INFO     LOG_S(INFO)
#define DCORE_LOG_DEBUG    LOG_S(DEBUG)
#define DCORE_LOG_ERROR    LOG_S(ERROR)
#define DCORE_LOG_CRITICAL LOG_S(CRITICAL)
#define DCORE_LOG_WARNING  LOG_S(WARNING)

// namespace dcore
// {
// 	template<typename... Args>
// 	void LogF(const char *fmt, Args... args)
// 	{

// 	}
// }
