#include "config.hpp"
#include "drawables.hpp"
#include "globals.hpp"
#include "timer.hpp"
#include <chrono>
#include <clocale>
#include <ctime>
#include <ncurses.h>
#include <pwd.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>

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

static std::string get_home_dir() {
    const auto pw = getpwuid(getuid());
    if (pw == nullptr)
        return "";
    return pw->pw_dir;
}

void draw() {
    using namespace std::chrono_literals;
    chronic::oneshot_timer weather_update_timer(0s);
    chronic::oneshot_timer timestamp_update_timer(0s);

    while (!chronic::globals::exiting.load()) {
        if (weather_update_timer.elapsed()) {
            chronic::globals::weather.store(chronic::weather::query());
            weather_update_timer.set(10min);
        } else if (timestamp_update_timer.elapsed()) {
            const auto now = time(nullptr);
            const auto *time  = localtime(&now);
            const int x = COLS / 2 - chronic::drawables::timestamp::width / 2;
            const int y = LINES / 2 - chronic::drawables::timestamp::height / 2;
            const int color = chronic::globals::color.load();
            chronic::drawables::timestamp timestamp(x, y, time, color);
            chronic::drawables::weather weatherstamp(chronic::globals::weather.load(), color);
            erase();
            timestamp.draw();
            weatherstamp.draw();
            refresh();
            timestamp_update_timer.set(1s);
        } else {
            continue;
        }
    }
}

int main() {
    initialize();

    const std::string config_path = get_home_dir() + "/.chronicrc";
    const chronic::config config = chronic::config::parse(config_path.c_str());
    chronic::globals::color.store(config.get_color());
    chronic::globals::bold.store(config.is_bold());

    {
        std::lock_guard<std::mutex> location_lock(chronic::globals::location_mutex);
        chronic::globals::location = config.get_location();
    }

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
