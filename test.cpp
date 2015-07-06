/* ---------------------------------------------------------------------- *
 * test.cpp
 * ---------------------------------------------------------------------- */

#include <iostream>
#include <fstream>
#include <assert.h>
#include "src/triangulation.hpp"
#include "src/flipgraph.hpp"
#include "src/functions.hpp"
#include "src/options.hpp"

/* ---------------------------------------------------------------------- *
 * helper functions
 * ---------------------------------------------------------------------- */

void print_list(std::vector<int> &list) {
    int l = (int) list.size();
    for (int i = 0; i < l; ++i) {
        std::cout << list[i] << " ";
    }
    std::cout << std::endl;
}

/* ---------------------------------------------------------------------- *
 * dominant canonical triangulations
 * ---------------------------------------------------------------------- */

void dominant_canonical_triangulations(int n) {
    assert(n >= 6);

    Triangulation canonical(n);
    Code canonical_code(canonical);

    Triangulation twin_star(n, TRIANGULATION_DOMINANT_TWIN_STAR);
    Code twin_star_code(twin_star);

    Triangulation zig_zag(n, TRIANGULATION_DOMINANT_ZIG_ZAG);
    Code zig_zag_code(zig_zag);

    Triangulation binary_tree(n, TRIANGULATION_DOMINANT_BINARY_TREE);
    Code binary_tree_code(binary_tree);

    FlipGraph flip_graph;
    flip_graph.compute(n);

    FlipGraph::Graph graph = flip_graph.graph();
    int size = graph.size();

    std::vector<int> bounds;
    std::vector<int> dominant_bounds;
    std::vector<int> histogram;

    for (int i = 0; i < size; ++i) {
        distance_histogram(graph, i, histogram);

        int distance = (int) histogram.size() - 1;
        int last = histogram[distance];
        int bound = (last == 1) ? 2 * distance - 1 : 2 * distance;

        if (bounds.size() <= bound) {
            bounds.resize(bound + 1, 0);
        }
        bounds[bound]++;

        Triangulation triangulation(flip_graph.code(i));
        if (triangulation.has_dominant_vertex()) {
            if (dominant_bounds.size() <= bound) {
                dominant_bounds.resize(bound + 1, 0);
            }
            dominant_bounds[bound]++;
        }

        if (i == 0) {
            std::cout << "canonical : " << bound << std::endl;
        }
        if (flip_graph.code(i) == twin_star_code) {
            std::cout << "twin star: " << bound << std::endl;
        }
        if (flip_graph.code(i) == zig_zag_code) {
            std::cout << "zig zag: " << bound << std::endl;
        }
        if (flip_graph.code(i) == binary_tree_code) {
            std::cout << "binary tree: " << bound << std::endl;
        }
    }

    std::cout << bounds.size() - 1 << std::endl;
    print_list(bounds);
    std::cout << dominant_bounds.size() - 1 << std::endl;
    print_list(dominant_bounds);
}

/* ---------------------------------------------------------------------- *
 * main function
 * ---------------------------------------------------------------------- */

int main(int argc, char *argv[]) {
    char *option_n = get_cmd_option(argc, argv, "-n");
    int n = (option_n) ? std::stoi(option_n) : 4;

    FlipGraph flip_graph;
    flip_graph.compute(n);

    std::vector<int> histogram;
    distance_histogram(flip_graph.graph(), 0, histogram);

    print_list(histogram);

    return 0;
}

/* ---------------------------------------------------------------------- *
 * end of file
 * ---------------------------------------------------------------------- */