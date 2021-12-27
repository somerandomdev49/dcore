#pragma once
#include <queue>
#include <string>
#include <dcore/Launch.hpp>
#include <stdarg.h>

#define DCORE_FORMAT_STRING __attribute__((format(printf, 1, 2)))

namespace dcore
{
	/**
	 * @brief Interface for logging on a single frame.
	 */
	class FrameLog
	{
	public:
		static void SLog(const std::string &message);
		static void SLogF(const char *format, ...) DCORE_FORMAT_STRING;
		void Log(const std::string &message);
		void LogF(const char *format, ...);
		void LogFV(const char *format, va_list va);

		const std::vector<std::string> &GetQueue() const;
		void Flush();

		static FrameLog *Instance();

	private:
		friend class launch::Launch;
		static void SetInstance(FrameLog *newInstance);
		std::vector<std::string> Queue_;
	};
} // namespace dcore
