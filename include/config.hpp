#if !defined(CHRONIC_CONFIG_HEADER)
#define CHRONIC_CONFIG_HEADER
#pragma once

#include <string>

namespace chronic {
    class config {
    public:
        static config parse(const char *path);
        int get_color() const;
        bool is_bold() const;
        const std::string& get_location() const;
    private:
        config(int c, bool b, std::string&& l);
    private:
        int color;
        bool bold;
        std::string location;
    };
}

#endif
