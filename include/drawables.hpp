#if !defined(CHRONIC_DRAWABLES_HEADER)
#define CHRONIC_DRAWABLES_HEADER
#pragma once

#include "constants.hpp"
#include <ctime>

namespace chronic {
    namespace drawables {

        struct number {
            int x, y;
            int digit;
            int color;

            number(int x, int y, int digit, int color);
            void draw();

            static constexpr int width = 6;
            static constexpr int height = 5;
        };

        struct progressbar {
            int x, y;
            double percent;
            int color;

            progressbar(int x, int y, int n, int color);
            void draw();

            static constexpr int width = (number::width * 4 + 10) * 2;
            static constexpr int height = 1;
        };

        struct timestamp {
            int x, y;
            number digits[5];
            int year, month, day, weekday;
            int color;
            progressbar progress;

            timestamp(int x, int y, const tm *t, int color);
            void draw();

            static constexpr int width = progressbar::width / 2;
            static constexpr int height = number::height + 6;
        };

    }
}

#endif
