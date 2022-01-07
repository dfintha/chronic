#include "drawables.hpp"
#include "globals.hpp"
#include <chrono>
#include <clocale>
#include <ctime>
#include <ncurses.h>
#include <thread>

static void initialize() {
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    curs_set(0);

    if (has_colors() == TRUE) {
        start_color();
        use_default_colors();

        init_pair(10, -1, COLOR_BLACK);
        init_pair(11, -1, COLOR_RED);
        init_pair(12, -1, COLOR_GREEN);
        init_pair(13, -1, COLOR_YELLOW);
        init_pair(14, -1, COLOR_BLUE);
        init_pair(15, -1, COLOR_MAGENTA);
        init_pair(16, -1, COLOR_CYAN);
        init_pair(17, -1, COLOR_WHITE);

        init_pair(20, COLOR_BLACK, -1);
        init_pair(21, COLOR_RED, -1);
        init_pair(22, COLOR_GREEN, -1);
        init_pair(23, COLOR_YELLOW, -1);
        init_pair(24, COLOR_BLUE, -1);
        init_pair(25, COLOR_MAGENTA, -1);
        init_pair(26, COLOR_CYAN, -1);
        init_pair(27, COLOR_WHITE, -1);
    }
}

void draw() {
    static constexpr clock_t clocks_per_10min = CLOCKS_PER_SEC * 60 * 10;
    static clock_t previous_timepoint;

    while (!chronic::globals::exiting.load()) {
        const auto now = time(nullptr);
        const auto *time  = localtime(&now);

        clock_t current_timepoint = clock();
        if (current_timepoint - previous_timepoint > clocks_per_10min) {
            chronic::globals::weather.store(chronic::weather::query());
            previous_timepoint = current_timepoint;
        }

        const int x = COLS / 2 - chronic::drawables::timestamp::width / 2;
        const int y = LINES / 2 - chronic::drawables::timestamp::height / 2;
        const int color = chronic::globals::color.load();
        chronic::drawables::timestamp timestamp(x, y, time, color);
        chronic::drawables::weather weatherstamp(chronic::globals::weather.load(), color);

        erase();
        timestamp.draw();
        weatherstamp.draw();
        refresh();

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    initialize();
    chronic::globals::weather.store(chronic::weather::query());

    std::thread loop {draw};

    while (!chronic::globals::exiting.load()) {
        int input = getch();

        switch (input) {
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
                chronic::globals::color.store(input - '0');
                break;

            case 'q':
            case 'Q':
                chronic::globals::exiting.store(true);
                break;

            case 'b':
            case 'B':
                chronic::globals::bold.store(!chronic::globals::bold.load());
                break;
        }
    }

    loop.join();
    endwin();
    return 0;
}
