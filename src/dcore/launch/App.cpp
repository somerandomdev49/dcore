#include <dcore/Core/Application.hpp>
#include <dcore/Core/Log.hpp>

dcore::Application::InfoT dcore::Application::Info {};

void dcore::Application::InfoT::SetError(bool critical, const std::string &msg,
                                         const std::string &title)
{
	Errors_.push_back(Error {critical, msg, title});
	DCORE_LOG_ERROR << title << " - " << msg;
	if(critical) exit(1);
}
