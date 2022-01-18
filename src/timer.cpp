#include "timer.hpp"

chronic::oneshot_timer::oneshot_timer(std::chrono::seconds seconds) {
    set(seconds);
}

bool chronic::oneshot_timer::elapsed() const {
    return std::chrono::system_clock::now() - set_time >= duration;
}

void chronic::oneshot_timer::set(std::chrono::seconds seconds) {
    this->duration = seconds;
    set_time = std::chrono::system_clock::now();
}
