/* ---------------------------------------------------------------------- *
 * functions.hpp
 * ---------------------------------------------------------------------- */

#ifndef __FGG_FUNCTIONS__
#define __FGG_FUNCTIONS__

#include <vector>

typedef std::vector<std::vector<int> > Graph;

void distance_list(const Graph& graph, int vertex, std::vector<int>& distances);
void distance_list(const Graph& graph, std::vector<int>& vertices, std::vector<int>& distances);

// computes a histogram of the shortest distances to one of the specified vertices
void distance_histogram(const Graph& graph, int vertex, std::vector<int>& histogram);
void distance_histogram(const Graph& graph, std::vector<int>& vertices, std::vector<int>& histogram);

// computes the eccentricity of the vertex with the specified index in the given graph
int eccentricity(const Graph& graph, int vertex);

// computes the diameter of the specified graph
int graph_diameter(const Graph& graph);

 #endif

/* ---------------------------------------------------------------------- *
 * end of file
 * ---------------------------------------------------------------------- */