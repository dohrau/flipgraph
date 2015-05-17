/* ---------------------------------------------------------------------- *
 * test.cpp
 * ---------------------------------------------------------------------- */

#include "src/options.hpp"
#include "src/flipgraph.hpp"
#include "src/functions.hpp"

#include <iostream>
#include <ctime>
#include <algorithm>
#include <vector>
#include <utility>
#include <cassert>
#include <string>
#include <fstream>

typedef Flip_graph::Graph Graph;

/* ---------------------------------------------------------------------- *
 * some helpful functions
 * ---------------------------------------------------------------------- */

void print_list(std::vector<int>& list) {
    int l = (int) list.size();
    for (int i = 0; i < l; ++i) {
        std::cout << list[i] << " ";
    }
    std::cout << std::endl;
}

void read_code(std::vector<unsigned char>& code) {
    std::string line;
    std::getline (std::cin, line);

    int n = 0;
    int i = 0;
    do {
        n *= 10;
        n += (int) line[i] - '0';
    } while (line[++i] != ' ');

    int m = 2*(3*n - 6);
    int l = n + m + 1;

    code.resize(l, 0);
    code[0] = (unsigned char) n;

    for (int j = i; j < i+l-1; ++j) {
        unsigned char c = line[j];
        if (c != ' ') { code[j-i] = c - 'a' + 1;}
    }
}

void to_graph(Triangulation& triangulation, Graph& graph) {
    int n = triangulation.order();
    graph.clear();
    graph.resize(n);

    for (int i = 0; i < n; ++i) {
        Vertex* vertex = triangulation.vertex(i);
        vertex->set_label(i);
    }

    for (int i = 0; i < n; ++i) {
        Vertex* vertex = triangulation.vertex(i);
        Halfedge* first = vertex->halfedge();
        Halfedge* current = first;
        do {
            graph[i].push_back(current->target()->label());
            current = current->twin()->next();
        } while (current != first);
    }
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
                for (int tb = ta+1; tb < size; ++tb) {
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

void speed_test() {
    for (int n = 7; n <= 13; ++n) {
        for (int i = 0; i <= 10; ++i) {

            clock_t start_time = clock();
            Flip_graph flip_graph;
            flip_graph.compute(n);
            clock_t end_time = clock();
            double elapsed = double(end_time - start_time) / CLOCKS_PER_SEC;

            std::cout << n << ", " << elapsed << std::endl;
        }
    }
}

/* ---------------------------------------------------------------------- *
 * main function
 * ---------------------------------------------------------------------- */

int main(int argc, char* argv[]) {
    //int n = 12;
    int n = 13;

    Flip_graph flip_graph;
    flip_graph.compute(n);

    int size = (int) flip_graph.graph().size();


    // precompute distances
    std::vector<std::vector<int> > distances(size);
    for (int i = 0; i < size; ++i) {
        distance_list(flip_graph.graph(), i, distances[i]);
    }

    int eleven = 0;
    int ten = 0;

    for (int i = 0; i < size; ++i) {
        std::vector<int> histogram;
        list_to_histogram(distances[i], histogram);
        int distance = (int) histogram.size() - 1;
        int last = histogram[distance];
        int bound = (last == 1) ? 2*distance - 1 : 2*distance;
        //if (bound > 12) { continue; }
        if (bound > 14) { continue; }

        // triangulations in boundary
        std::vector<int> list;
        for (int k = 0; k < size; ++k) {
            //assert (distances[i][k] <= 6);
            assert (distances[i][k] <= 7);
            //if (distances[i][k] == 6) {
            if (distances[i][k] == 7) {
                list.push_back(k);
            }
        }
        int l = (int) list.size();
        std::cout << "l = " << l << std::endl;

        for (int j = i; j < size; ++j) {
            int five = 0;
            int six = 0;
            for (int k = 0; k < l; ++k) {
                //if (distances[j][list[k]] == 6) { six++; }
                if (distances[j][list[k]] == 7) { six++; }
                //if (distances[j][list[k]] == 5) { five++; }
                if (distances[j][list[k]] == 6) { five++; }
            }
            if (six == 1 && five > 0) { eleven++; }
            if (six == 0 && five > 1) { ten++; }
        }
    }

    std::cout << "eleven = " << eleven << std::endl;
    std::cout << "ten    = " << ten    << std::endl;

    return 0;
}

int test(int argc, char* argv[]) {
    // option -n: number of vertices
    char* option_n = get_cmd_option(argc, argv, "-n");
    int n = (option_n) ? std::stoi(option_n) : 6;

    Flip_graph flip_graph;
    flip_graph.compute(n);

    int size = (int) flip_graph.graph().size();
    std::vector<int> histogram;

    std::vector<std::pair<int, std::pair<int, int> > > list;

    for (int index = 0; index < size; ++index) {
        distance_histogram(flip_graph.graph(), index, histogram);
        int distance = (int) histogram.size() - 1;
        int last = histogram[distance];
        int prev = (last == 1) ? histogram[distance-1] : last;
        int pairs = last * prev;
        int bound = (last == 1) ? 2*distance-1 : 2*distance;

        list.push_back(std::make_pair(bound, std::make_pair(pairs, index)));
    }

    sort(list.begin(), list.end());
    int best_bound = list[0].first;

    for (int i = 0; i < size; ++i) {
        int bound = list[i].first;
        int pairs = list[i].second.first;
        int index = list[i].second.second;
        if (bound > best_bound) break;

        const Code& code = flip_graph.code(index);
        Triangulation triangulation(code);

        std::ofstream file_stream;
        file_stream.open("graph_"+std::to_string(i)+".dot");
        triangulation.write_to_stream(file_stream);
        file_stream.close();
    }
    
    return 0;
}

/* ---------------------------------------------------------------------- *
 * end of file
 * ---------------------------------------------------------------------- */