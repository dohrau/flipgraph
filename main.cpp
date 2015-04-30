#include "src/options.hpp"
#include "src/flipgraph.hpp"
#include "src/triangulation.hpp"
#include "src/functions.hpp"

#include <vector>
#include <iostream>
#include <fstream>
#include <cassert>

const int MODE_GENERATE = 0;
const int MODE_DIAMETER = 1;
const int MODE_TEST = 42;

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
        } else if (strcmp(option_m, "test") == 0) {
            mode = MODE_TEST;
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

    Flip_graph flip_graph;
    flip_graph.compute(n);

    switch (mode) {
        case MODE_GENERATE: {
            flip_graph.write_to_stream(output_stream);
            break;
        }
        case MODE_DIAMETER: {
            int diameter = graph_diameter(flip_graph.graph());
            output_stream << diameter << std::endl;
            break;
        }
        case MODE_TEST: {
            int size = (int) flip_graph.graph().size();
            Code code = flip_graph.code(size - 1);
            Triangulation triangulation(code);
            triangulation.write_to_stream(output_stream);
            break;
        }
    }

    // close output stream
    if (file_output) { file_stream.close(); }

    return 0;
}