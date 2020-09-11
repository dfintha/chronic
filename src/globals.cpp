#include "globals.hpp"
#include <ncurses.h>

std::atomic_int chronic::globals::color(COLOR_CYAN);
std::atomic_bool chronic::globals::bold(true);
std::atomic_bool chronic::globals::exiting(false);
