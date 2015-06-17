/* ---------------------------------------------------------------------- *
 * test.cpp
 * ---------------------------------------------------------------------- */

#include <iostream>
#include "src/triangulation.hpp"

/* ---------------------------------------------------------------------- *
 * main function
 * ---------------------------------------------------------------------- */

int main(int argc, char *argv[]) {
    int n = 6;

    Triangulation triangulation(n, TRIANGULATION_DOMINANT_DOUBLE_FAN);
    Code code(triangulation);

    triangulation = *(new Triangulation(code));

    //code.write_to_stream(std::cout);
    triangulation.write_to_stream(std::cout);

    return 0;
}

/* ---------------------------------------------------------------------- *
 * end of file
 * ---------------------------------------------------------------------- */