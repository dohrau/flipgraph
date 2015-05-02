/* ---------------------------------------------------------------------- *
 * test.cpp
 * ---------------------------------------------------------------------- */

#include "src/options.hpp"
#include "src/flipgraph.hpp"
#include "src/functions.hpp"

#include <iostream>
#include <ctime>
#include <cassert>

/* ---------------------------------------------------------------------- *
 * constants
 * ---------------------------------------------------------------------- */

const int MINIMUM_N = 4;
const int DEFAULT_N = 4;

/* ---------------------------------------------------------------------- *
 * main function
 * ---------------------------------------------------------------------- */

int main(int argc, char* argv[]) {
    // option -n: number of vertices
    char* option_n = get_cmd_option(argc, argv, "-n");
    int n = (option_n) ? std::stoi(option_n) : DEFAULT_N;
    assert(n >= MINIMUM_N);

    // option -t: show elapsed time
    bool show_time = cmd_option_exists(argc, argv, "-t");

    // compute flip graph
    clock_t start_time = clock();
    Flip_graph flip_graph;
    flip_graph.compute(n);
    clock_t end_time = clock();

    if (show_time) {
        double elapsed = double(end_time - start_time) / CLOCKS_PER_SEC;
        std::cout << "flip graph generated in " << elapsed << "s" << std::endl;
    }

    return 0;
}

/* ---------------------------------------------------------------------- *
 * end of file
 * ---------------------------------------------------------------------- */