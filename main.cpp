#include "src/options.hpp"
#include "src/flipgraph.hpp"
#include "src/triangulation.hpp"
#include "src/functions.hpp"

#include <vector>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cassert>

// #define NDEBUG

const int MODE_GENERATE = 0;
const int MODE_DIAMETER = 1;
const int MODE_TRIANGULATION = 2;
const int MODE_TEST = 99;
const int DEFAULT_MODE = MODE_GENERATE;

const int FORMAT_PLAIN = 0;
const int FORMAT_CODE = 1;
const int FORMAT_DOT = 2;
const int DEFAULT_FORMAT = FORMAT_PLAIN;

const int DEFAULT_N = 4;
const int DEFAULT_INDEX = 1;

int main(int argc, char* argv[]) {
    // option -m: mode
    char* option_m = get_cmd_option(argc, argv, "-m");
    int mode = DEFAULT_MODE;
    if (option_m) {
        if (strcmp(option_m, "generate") == 0) {
            mode = MODE_GENERATE;
        } else if (strcmp(option_m, "diameter") == 0) {
            mode = MODE_DIAMETER;
        } else if (strcmp(option_m, "triangulation") == 0) {
            mode = MODE_TRIANGULATION;
        } else if (strcmp(option_m, "test") == 0) {
            mode = MODE_TEST;
        }
    }

    // option -n: number of vertices
    char* option_n = get_cmd_option(argc, argv, "-n");
    int n = (option_n) ? std::stoi(option_n) : DEFAULT_N;

    // option -i: index
    char* option_i = get_cmd_option(argc, argv, "-i");
    int index = (option_i) ? std::stoi(option_i) : DEFAULT_INDEX;

    // option -r: reverse indices
    bool reverse = cmd_option_exists(argc, argv, "-r");
    
    // option -o: output file
    char* option_o = get_cmd_option(argc, argv, "-o");
    bool file_output = false;
    std::ofstream file_stream;
    if (option_o) {
        file_output = true;
        file_stream.open(option_o);
    }
    std::ostream& output_stream = file_output ? file_stream : std::cout;

    // option -f: output format
    char* option_f = get_cmd_option(argc, argv, "-f");
    int format = DEFAULT_FORMAT;
    if (option_f) {
        if (strcmp(option_f, "plain") == 0) {
            format = FORMAT_PLAIN;
        } else if (strcmp(option_f, "code") == 0) {
            format = FORMAT_CODE;
        } else if (strcmp(option_f, "dot") == 0) {
            format = FORMAT_DOT;
        }
    }

    // option -t: show elapsed time
    int show_time = cmd_option_exists(argc, argv, "-t");

    // compute flip graph
    clock_t start_time = clock();
    Flip_graph flip_graph;
    flip_graph.compute(n);
    clock_t end_time = clock();

    if (show_time) {
        double elapsed = double(end_time - start_time) / CLOCKS_PER_SEC;
        std::cout << "flip graph generated in " << elapsed << "s" << std::endl;
    }

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
        case MODE_TRIANGULATION: {
            int size = (int) flip_graph.graph().size();
            int i = reverse ? size - index : index - 1;
            assert(i >= 0 && i < size);
            Code code = flip_graph.code(i);
            if (format == FORMAT_PLAIN || format == FORMAT_CODE) {
                code.write_to_stream(output_stream);
            } else {
                Triangulation triangulation(code);
                triangulation.write_to_stream(output_stream);
            }
            break;
        }
        case MODE_TEST: {
            break;
        }
    }

    // close output stream
    if (file_output) { file_stream.close(); }

    return 0;
}