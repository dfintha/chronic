#if !defined(CHRONIC_TIMER_HEADER)
#define CHRONIC_TIMER_HEADER
#pragma once

#include <chrono>

namespace chronic {
    class oneshot_timer {
    public:
        oneshot_timer(std::chrono::seconds seconds);
        bool elapsed() const;
        void set(std::chrono::seconds seconds);
    private:
        std::chrono::time_point<std::chrono::system_clock> set_time;
        std::chrono::seconds duration;
    };
}

#endif
