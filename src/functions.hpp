/* ---------------------------------------------------------------------- *
 * functions.hpp
 * ---------------------------------------------------------------------- */

#ifndef __FGG_FUNCTIONS__
#define __FGG_FUNCTIONS__

#include <vector>

typedef std::vector<std::vector<int> > Graph;

// coputes histogram from list of distances
void list_to_histogram(std::vector<int>& distances, std::vector<int>& histogram);

// computes the distances for all vertices to the specified vertex
void distance_list(const Graph& graph, int vertex, std::vector<int>& distances);

// computes the distances for all vertices to the set of specified vertices.
void distance_list(const Graph& graph, std::vector<int>& vertices, std::vector<int>& distances);

// computes a histogramm of the distances to the specified vertex
void distance_histogram(const Graph& graph, int vertex, std::vector<int>& histogram);

// computes a histogram of the distances to one of the set of specified vertices
void distance_histogram(const Graph& graph, std::vector<int>& vertices, std::vector<int>& histogram);

// computes the eccentricity of the vertex with the specified index in the given graph
int eccentricity(const Graph& graph, int vertex);

// computes the diameter of the specified graph
int graph_diameter(const Graph& graph);

 #endif

/* ---------------------------------------------------------------------- *
 * end of file
 * ---------------------------------------------------------------------- */