/* ---------------------------------------------------------------------- *
 * options.hpp
 * ---------------------------------------------------------------------- */

#ifndef __FGG_OPTIONS__
#define __FGG_OPTIONS__

#include <string>

// returns the value of the specified option or 0 if it does not exist
char *get_cmd_option(int argc, char *argv[], const std::string &option);

// returns whether the specified options exists or not
bool cmd_option_exists(int argc, char *argv[], const std::string &option);

#endif

/* ---------------------------------------------------------------------- *
 * end of file
 * ---------------------------------------------------------------------- */