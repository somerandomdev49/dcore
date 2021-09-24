#pragma once
#include <dcore/Core/Log.hpp>
#include <cstdlib>
#include <cstdio>
#include <cassert>

namespace dcore
{
    inline bool internal_Assert(const char *msg, const char *s, const char *file, int line)
    {
        DCORE_LOG_ERROR << "ASSERTION FAILED: " << msg;
        DCORE_LOG_ERROR << "  " << file << ':' << line << " - '" << s << '\'';
        std::exit(1);
        return false;
    }
} 
#define DCORE_ASSERT(expr, message) ((void)((expr) || dcore::internal_Assert((message), #expr, __FILE__, __LINE__)))

