/* ---------------------------------------------------------------------- *
 * test.cpp
 * ---------------------------------------------------------------------- */

#include "src/options.hpp"
#include "src/flipgraph.hpp"
#include "src/functions.hpp"

#include <iostream>
#include <ctime>
#include <cassert>

typedef Flip_graph::Graph Graph;

/* ---------------------------------------------------------------------- *
 * constants
 * ---------------------------------------------------------------------- */

const int MINIMUM_N = 4;
const int DEFAULT_N = 4;

/* ---------------------------------------------------------------------- *
 * print vector
 * ---------------------------------------------------------------------- */

void print_list(std::vector<int>& list) {
    int l = (int) list.size();
    for (int i = 0; i < l; ++i) {
        std::cout << list[i] << " ";
    }
    std::cout << std::endl;
}

/* ---------------------------------------------------------------------- *
 * distance via single triangulation
 * ---------------------------------------------------------------------- */

void single_distance(const Graph& graph) {
    int size = (int) graph.size();

    std::vector<int> bounds;
    std::vector<int> histogram;

    for (int i = 0; i < size; ++i) {
        distance_histogram(graph, i, histogram);

        int distance = (int) histogram.size() - 1;
        int last = histogram[distance];
        int bound = (last == 1) ? 2*distance - 1 : 2*distance;
        
        if (bounds.size() <= bound) {
            bounds.resize(bound+1, 0);
        }
        bounds[bound]++;

        if (i == 0) {
            std::cout << bound << std::endl;
        }
    }

    std::cout << bounds.size()-1 << std::endl;
    print_list(bounds);
}

void double_distance(const Graph& graph) {
    int size = (int) graph.size();

    std::vector<int> bounds;
    std::vector<std::vector<int> > distances(size);

    // precompute distances
    for (int i = 0; i < size; ++i) {
        distance_list(graph, i, distances[i]);
    }

    // check all pairs c1 and c2
    for (int c1 = 0; c1 < size; ++c1) {
        for (int c2 = c1; c2 < size; ++c2) {
            // find worst pair for c1 and c2
            int bound = 0;
            for (int ta = 0; ta < size; ++ta) {
                for (int tb = ta; tb < size; ++tb) {
                    int d_a1 = distances[ta][c1];
                    int d_a2 = distances[ta][c2];
                    int d_b1 = distances[tb][c1];
                    int d_b2 = distances[tb][c2];
                    int best = std::min(d_a1 + d_b1, d_a2 + d_b2);
                    bound = std::max(bound, best);
                }
            }
            //
            if (bounds.size() <= bound) {
                bounds.resize(bound+1, 0);
            }
            bounds[bound]++;
        }
    }

    print_list(bounds);
}

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

    //single_distance(flip_graph.graph());
    double_distance(flip_graph.graph());

    return 0;
}

/* ---------------------------------------------------------------------- *
 * end of file
 * ---------------------------------------------------------------------- */