#pragma once
#include <string>

namespace dcore::util
{
    class Debug
    {
    public:
        static std::string Demangle(const std::string &name);
    };
}
