/* ---------------------------------------------------------------------- *
 * test.cpp
 * ---------------------------------------------------------------------- */

#include <iostream>
#include "src/triangulation.hpp"
#include "src/flipgraph.hpp"
#include "src/functions.hpp"

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
 * main function
 * ---------------------------------------------------------------------- */

int main(int argc, char *argv[]) {
    int n = 6;

    Triangulation double_fan(n, TRIANGULATION_DOMINANT_DOUBLE_FAN);
    Code double_fan_code(double_fan);

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
        if (flip_graph.code(i) == double_fan_code) {
            std::cout << "double fan: " << bound << std::endl;
        }
        if (flip_graph.code(i) == binary_tree_code) {
            std::cout << "binary tree: " << bound << std::endl;
        }
    }

    std::cout << bounds.size() - 1 << std::endl;
    print_list(bounds);
    print_list(dominant_bounds);

    return 0;
}

/* ---------------------------------------------------------------------- *
 * end of file
 * ---------------------------------------------------------------------- */