#include "src/options.hpp"
#include "src/flipgraph.hpp"
#include "src/functions.hpp"

#include <vector>
#include <iostream>
#include <fstream>
#include <cassert>

const int MODE_GENERATE = 0;
const int MODE_DIAMETER = 1;

const int DEFAULT_MODE = MODE_GENERATE;
const int DEFAULT_N = 4;

int main(int argc, char* argv[]) {
    // option -m: mode
    char* option_m = get_cmd_option(argc, argv, "-m");
    int mode = DEFAULT_MODE;
    if (option_m) {
        if (strcmp(option_m, "generate") == 0) {
            mode = MODE_GENERATE;
        } else if (strcmp(option_m, "diameter") == 0) {
            mode = MODE_DIAMETER;
        }
    }

    // option -n: number of vertices
    char* option_n = get_cmd_option(argc, argv, "-n");
    int n = (option_n) ? std::stoi(option_n) : DEFAULT_N;
    
    // option -o: output file
    char* option_o = get_cmd_option(argc, argv, "-o");
    bool file_output = false;
    std::ofstream file_stream;
    if (option_o) {
        file_output = true;
        file_stream.open(option_o);
    }
    std::ostream& output_stream = file_output ? file_stream : std::cout;

    std::vector<std::vector<int> > graph;
    compute_flip_graph(n, graph);

    switch (mode) {
    case MODE_GENERATE:
        write_flip_graph(graph, output_stream);
        break;
    case MODE_DIAMETER:
        int diameter = graph_diameter(graph);
        output_stream << diameter << std::endl;
        break;
    }

    // close output stream
    if (file_output) { file_stream.close(); }

    return 0;
}