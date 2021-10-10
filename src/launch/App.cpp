#include <dcore/Core/Application.hpp>

auto dcore::Application::Info{};

void dcore::Application::InfoT::SetError(bool critical, const std::string &msg, const std::string &title)
{
    Errors_.push_back(Error{critical, msg, title});
}
