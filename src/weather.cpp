#include "globals.hpp"
#include "weather.hpp"
#include <cstdio>
#include <cstring>
#include <curl/curl.h>
#include <string>
#include <vector>

static size_t curl_to_string(char *pointer, size_t, size_t length, void *vec) {
    if (vec == nullptr || length == 0)
        return 0;
    std::vector<char>& subject = *reinterpret_cast<std::vector<char> *>(vec);
    subject.insert(subject.end(), pointer, pointer + length);
    return length;
}

static const chronic::weather::information failed_weather_info = {
    "Failed to Retrieve Weather Information",
    "Failed to Retrieve Weather Information",
    0, 0, 0, 0,
    ""
};

chronic::weather::information chronic::weather::query() {
    CURL *curl = curl_easy_init();
    if (curl == nullptr)
        return failed_weather_info;

    std::vector<char> received;

    {
        std::lock_guard<std::mutex> location_lock(chronic::globals::location_mutex);
        std::string url =
            "https://wttr.in/" +
            chronic::globals::location +
            "?T&format=\"%l|%C|%t+%f+%h|%w\"";
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    }

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_to_string);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &received);

    auto code = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    if (code != CURLE_OK)
        return failed_weather_info;

    std::string response = received.data();
    chronic::weather::information result;
    const char *pointer = response.c_str();
    ++pointer;

    const size_t location_length = size_t(strchr(pointer, '|') - pointer);
    strncpy(result.location, pointer, location_length);
    result.location[location_length] = '\0';
    pointer += location_length + 1;

    const size_t condition_length = size_t(strchr(pointer, '|') - pointer);
    strncpy(result.condition, pointer, condition_length);
    result.condition[condition_length] = '\0';
    pointer += condition_length + 2;

    sscanf(
        pointer,
        "%d°C %d°C %d%%",
        &result.temperature_real,
        &result.temperature_feel,
        &result.humidity_percent
    );

    pointer = strchr(pointer, '|') + 1;

    memset(result.wind_direction, 0x00, sizeof(result.wind_direction));
    char *direction = result.wind_direction;
    while (!isdigit(*pointer))
        *direction++ = *pointer++;
    sscanf(pointer, "%dkm/h", &result.wind_speed);

    return result;
}
