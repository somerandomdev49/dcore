#include <dcore/Core/FrameLog.hpp>
#include <stdarg.h>

namespace dcore
{
	static FrameLog *instFrameLog = nullptr;
	FrameLog *FrameLog::Instance()
	{
		if(instFrameLog == nullptr) instFrameLog = new FrameLog;
		return instFrameLog;
	}

	void FrameLog::SetInstance(FrameLog *newInstance) { instFrameLog = newInstance; }

	void FrameLog::SLog(const std::string &message) { FrameLog::Instance()->Log(message); }
	void FrameLog::SLogF(const char *format, ...)
	{
		va_list v;
		va_start(v, format);
		FrameLog::Instance()->LogFV(format, v);
		va_end(v);
	}

	void FrameLog::LogF(const char *format, ...)
	{
		va_list v;
		va_start(v, format);
		LogFV(format, v);
		va_end(v);
	}

	void FrameLog::LogFV(const char *format, va_list va)
	{
		auto size = std::vsnprintf(nullptr, 0, format, va);
		std::string output(size + 1, '\0');
		std::vsprintf(&output[0], format, va);
		Log(output);
	}

	void FrameLog::Log(const std::string &message) { Queue_.push_back(message); }

	void FrameLog::Flush() { Queue_.clear(); }

	const std::vector<std::string> &FrameLog::GetQueue() const { return Queue_; }

} // namespace dcore