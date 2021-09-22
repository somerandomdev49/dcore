#pragma once
#include <cstdlib>
#include <cstdio>
#include <cassert>

namespace dcore
{
    inline bool internal_Assert(const char *msg, const char *s, const char *file, int line)
    {
        std::fprintf(stderr, "ASSERTION FAILED: %s\n  %s:%d - '%s'", msg, file, line, s);
        std::exit(1);
        return false;
    }
} 
#define DCORE_ASSERT(expr, message) ((void)((expr) || dcore::internal_Assert((message), #expr, __FILE__, __LINE__)))

