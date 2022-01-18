#include "config.hpp"
#include <cstdio>
#include <cstring>
#include <ncurses.h>
#include <string>

class file_handle {
public:
    file_handle(FILE *ptr) : handle(ptr) { }
    ~file_handle() noexcept { if (handle != nullptr) fclose(handle); }
    operator FILE *() { return handle; }
private:
    FILE *handle;
};

chronic::config chronic::config::parse(const char *path) {
    int color = COLOR_CYAN;
    bool bold = false;

    char buffer[512] = {};
    file_handle file = fopen(path, "r");
    if (file == nullptr) {
        return chronic::config(color, bold);
    }

    while (fgets(buffer, 512, file) != nullptr) {
        const char *where = strchr(buffer, '=');
        if (where == nullptr)
            continue;

        const char *nl = strchr(where, '\n');
        const char *nul = strchr(where, '\0');
        const char *end = nl < nul ? nl : nul;

        const std::string key(buffer, size_t(where - buffer));
        const std::string value(where + 1, size_t(end - where - 1));
        const char *key_c = key.c_str();
        const char *value_c = value.c_str();

        if (strcmp(key_c, "color") == 0) {
            if (strcmp(value_c, "red") == 0) color = COLOR_RED;
            if (strcmp(value_c, "green") == 0) color = COLOR_GREEN;
            if (strcmp(value_c, "yellow") == 0) color = COLOR_YELLOW;
            if (strcmp(value_c, "blue") == 0) color = COLOR_BLUE;
            if (strcmp(value_c, "magenta") == 0) color = COLOR_MAGENTA;
            if (strcmp(value_c, "cyan") == 0) color = COLOR_CYAN;
            if (strcmp(value_c, "white") == 0) color = COLOR_WHITE;
        } else if (strcmp(key_c, "bold") == 0) {
            if (strcmp(value_c, "true") == 0) bold = true;
            if (strcmp(value_c, "false") == 0) bold = false;
        }
    }

    return chronic::config(color, bold);
}

int chronic::config::get_color() const {
    return color;
}

bool chronic::config::is_bold() const {
    return bold;
}

chronic::config::config(int c, bool b) : color(c), bold(b) {
}
