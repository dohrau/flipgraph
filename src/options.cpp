/* ---------------------------------------------------------------------- *
 * options.cpp
 * ---------------------------------------------------------------------- */

#include "options.hpp"

char *get_cmd_option(int argc, char *argv[], const std::string &option) {
    char **begin = argv;
    char **end = argv + argc;
    char **it = std::find(begin, end, option);
    return (it != end && ++it != end) ? *it : 0;
}

bool cmd_option_exists(int argc, char *argv[], const std::string &option) {
    char **begin = argv;
    char **end = argv + argc;
    return std::find(begin, end, option) != end;
}

/* ---------------------------------------------------------------------- *
 * end of file
 * ---------------------------------------------------------------------- */