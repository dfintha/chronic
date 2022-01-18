#include "globals.hpp"
#include <ncurses.h>

std::atomic_int chronic::globals::color(COLOR_CYAN);
std::atomic_bool chronic::globals::bold(true);
std::atomic_bool chronic::globals::exiting(false);
std::string chronic::globals::location("");
std::mutex chronic::globals::location_mutex;
std::atomic<chronic::weather::information> chronic::globals::weather({"", "", 0, 0, 0, 0, ""});
