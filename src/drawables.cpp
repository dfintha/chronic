#include "drawables.hpp"
#include "globals.hpp"
#include <cstring>
#include <ncurses.h>

chronic::drawables::number::number(int x, int y, int digit, int color) :
    x(x),
    y(y),
    digit(digit),
    color(color)
{ }

void chronic::drawables::number::draw() {
    for (int dx = 0; dx < width; ++dx) {
        for (int dy = 0; dy < height; ++dy) {
            move(y + dy, x + dx);
            const char at = chronic::constants::number[digit][dy * width + dx];
            if (at != ' ')
                attron(COLOR_PAIR(BGCOLOR(color)));
            addch(' ');
            attroff(COLOR_PAIR(BGCOLOR(color)));
        }
    }
}

chronic::drawables::timestamp::timestamp(int x, int y, const tm *t, int color) :
    x(x),
    y(y),
    digits{
        number(x, y, t->tm_hour / 10, color),
        number(x, y, t->tm_hour % 10, color),
        number(x, y, 10, color),
        number(x, y, t->tm_min / 10, color),
        number(x, y, t->tm_min % 10, color),
    },
    year(t->tm_year + 1900),
    month(t->tm_mon + 1),
    day(t->tm_mday),
    weekday(t->tm_wday),
    color(color),
    progress{x - timestamp::width / 2, y + timestamp::height - 1, 0, color}
{
    digits[1].x = digits[0].x + number::width + 2;
    digits[2].x = digits[1].x + number::width;
    digits[3].x = digits[2].x + number::width;
    digits[4].x = digits[3].x + number::width + 2;

    const bool leap = (year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0));
    const int days = leap? 366 : 365;
    const float percent = float(t->tm_yday) / float(days);
    progress.percent = percent;
}

static const char * weekday_to_string(int weekday) {
    switch (weekday) {
        case 1:
            return "Monday";
        case 2:
            return "Tuesday";
        case 3:
            return "Wednesday";
        case 4:
            return "Thursday";
        case 5:
            return "Friday";
        case 6:
            return "Saturday";
        case 0:
        case 7:
            return "Sunday";
        default:
            return "ERROR";
    }
}

void chronic::drawables::timestamp::draw() {
    char buffer[128] = {};

    if (chronic::globals::bold.load())
        attron(A_BOLD);

    for (int i = 0; i < 5; ++i) {
        digits[i].draw();
    }

    attron(COLOR_PAIR(FGCOLOR(color)));

    sprintf(buffer, "%04d-%02d-%02d", year, month, day);
    const int dn = int(strlen(buffer));
    const int dx = x + timestamp::width / 2 - dn / 2;
    const int dy = y + timestamp::height - 5;
    mvprintw(dy, dx, "%s", buffer);

    sprintf(buffer, "%s", weekday_to_string(weekday));
    const int wn = int(strlen(buffer));
    const int wx = x + timestamp::width / 2 - wn / 2;
    const int wy = y + timestamp::height - 4;
    mvprintw(wy, wx, "%s", buffer);

    sprintf(buffer, "%d%% of %04d", int(progress.percent * 100), year);
    const int yn = int(strlen(buffer));
    const int yx = x + timestamp::width / 2 - yn / 2;
    const int yy = y + timestamp::height - 2;
    mvprintw(yy, yx, "%s", buffer);

    attroff(COLOR_PAIR(FGCOLOR(color)));

    progress.draw();

    attroff(A_BOLD);
}

chronic::drawables::progressbar::progressbar(int x, int y, int n, int color) :
    x(x),
    y(y),
    percent(float(n)),
    color(color)
{ }

void chronic::drawables::progressbar::draw() {
    constexpr const int full = progressbar::width - 2;
    const int filled = int(full * percent);

    attron(COLOR_PAIR(FGCOLOR(color)));
    move(y, x + progressbar::width - 2);
    addch('|');
    move(y, x);
    addch('|');
    attroff(COLOR_PAIR(FGCOLOR(color)));

    attron(COLOR_PAIR(BGCOLOR(color)));
    for (int i = 0; i < filled; ++i)
        addch(' ');
    attroff(COLOR_PAIR(BGCOLOR(color)));
}
