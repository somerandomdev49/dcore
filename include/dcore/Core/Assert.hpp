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
        return false;
    }

    inline bool internal_AssertExit(const char *msg, const char *s, const char *file, int line)
    {
        internal_Assert(msg, s, file, line);
        std::exit(1);
        return false;
    }
} 

#define DCORE_ASSERT(expr, message) ((void)((expr) || dcore::internal_AssertExit((message), #expr, __FILE__, __LINE__)))
#define DCORE_ASSERT_RETURN(expr, message) if(!(expr)){dcore::internal_Assert((message), #expr, __FILE__, __LINE__);return;};
