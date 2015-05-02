/* ---------------------------------------------------------------------- *
 * functions.hpp
 * ---------------------------------------------------------------------- */

#ifndef __FGG_FUNCTIONS__
#define __FGG_FUNCTIONS__

#include <vector>

typedef std::vector<std::vector<int> > Graph;

// computes the eccentricity of the vertex with the specified index in the given graph
int vertex_eccentricity(const Graph& graph, int index);

// computes the diameter of the specified graph
int graph_diameter(const Graph& graph);

// computes a histogram of the shortest distances to one of the specified vertices
void distance_histogram(const Graph& graph, std::vector<int>& vertices, std::vector<int>& histogram);

 #endif

/* ---------------------------------------------------------------------- *
 * end of file
 * ---------------------------------------------------------------------- */