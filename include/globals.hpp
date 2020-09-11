#if !defined(CHRONIC_GLOBALS_HEADER)
#define CHRONIC_GLOBALS_HEADER
#pragma once

#include <atomic>

namespace chronic {
    namespace globals {
        extern std::atomic_int color;
        extern std::atomic_bool bold;
        extern std::atomic_bool exiting;
    }
}

#endif
