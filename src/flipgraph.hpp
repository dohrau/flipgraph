/* ---------------------------------------------------------------------- *
 * flipgraph.hpp
 *
 * author: jerome dohrau
 * ---------------------------------------------------------------------- */

#ifndef __FG_FLIP_GRAPH__
#define __FG_FLIP_GRAPH__

#include <vector>

// computes the flip graph on all triangulations with n vertices
void compute_flip_graph(int n, std::vector<std::vector<int> >& graph);

// writes the specified flip graph to the specified stream
void write_flip_graph(std::vector<std::vector<int> >& graph, std::ostream& output_stream);

#endif

/* ---------------------------------------------------------------------- *
 * end of file
 * ---------------------------------------------------------------------- */