#if !defined(CHRONIC_WEATHER_HEADER)
#define CHRONIC_WEATHER_HEADER
#pragma once

#include <string>

namespace chronic {
    namespace weather {
        struct information {
            char location[128];
            char condition[128];
            int temperature_real;
            int temperature_feel;
            int humidity_percent;
            int wind_speed;
            char wind_direction[4];
        };

        information query();
    }
}

#endif
