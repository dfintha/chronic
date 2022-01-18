#if !defined(CHRONIC_CONFIG_HEADER)
#define CHRONIC_CONFIG_HEADER
#pragma once

namespace chronic {
    class config {
    public:
        static config parse(const char *path);
        int get_color() const;
        bool is_bold() const;
    private:
        config(int c, bool b);
    private:
        int color;
        bool bold;
    };
}

#endif
