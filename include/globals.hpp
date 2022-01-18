#if !defined(CHRONIC_GLOBALS_HEADER)
#define CHRONIC_GLOBALS_HEADER
#pragma once

#include "weather.hpp"
#include <atomic>
#include <mutex>
#include <string>

namespace chronic {
    namespace globals {
        extern std::atomic_int color;
        extern std::atomic_bool bold;
        extern std::atomic_bool exiting;
        extern std::string location;
        extern std::mutex location_mutex;
        extern std::atomic<chronic::weather::information> weather;
    }
}

#endif
