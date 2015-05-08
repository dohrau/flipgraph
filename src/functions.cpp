/* ---------------------------------------------------------------------- *
 * functions.cpp
 * ---------------------------------------------------------------------- */

#include "functions.hpp"

#include <queue>
#include <utility>

void distance_list(const Graph& graph, int vertex, std::vector<int>& distances) {
    std::vector<int> vertices(1, vertex);
    distance_list(graph, vertices, distances);
}

void distance_list(const Graph& graph, std::vector<int>& vertices, std::vector<int>& distances) {
    int n = (int) graph.size();
    int l = (int) vertices.size();

    distances.clear();
    distances.resize(n, -1);

    std::queue<std::pair<int, int> > queue;
    for (int i = 0; i < l; ++i) {
        int vertex = vertices[i];
        if (distances[vertex] == -1) {
            distances[vertex] = 0;
            queue.push(std::make_pair(vertex, 0));
        }
    }

    while (!queue.empty()) {
        int vertex = queue.front().first;
        int distance = queue.front().second;
        queue.pop();

        int degree = (int) graph[vertex].size();
        for (int i = 0; i < degree; ++i) {
            int neighbor = graph[vertex][i];
            if (distances[neighbor] == -1) {
                distances[neighbor] = distance + 1;
                queue.push(std::make_pair(neighbor, distance + 1));
            }
        }
    }
}

void distance_histogram(const Graph& graph, int vertex, std::vector<int>& histogram) {
    std::vector<int> vertices(1, vertex);
    distance_histogram(graph, vertices, histogram);
}

void distance_histogram(const Graph& graph, std::vector<int>& vertices, std::vector<int>& histogram) {
    std::vector<int> distances;
    distance_list(graph, vertices, distances);

    int n = (int) graph.size();
    int maximum = -1;
    histogram.clear();
    for (int i = 0; i < n; ++i) {
        int distance = distances[i];
        if (maximum < distance) {
            histogram.resize(distance + 1, 0);
            maximum = distance;
        }
        histogram[distance]++;
    }
}

int eccentricity(const Graph& graph, int vertex) {
    std::vector<int> distances;
    distance_list(graph, vertex, distances);

    int n = (int) graph.size();
    int result = 0;
    for (int i = 0; i < n; ++i) {
        result = std::max(result, distances[i]);
    }

    return result;
}

int graph_diameter(const Graph& graph) {
    int n = (int) graph.size();
    int result = 0;

    for (int i = 0; i < n; ++i) {
        result = std::max(result, eccentricity(graph, i));
    }

    return result;
}

/* ---------------------------------------------------------------------- *
 * end of file
 * ---------------------------------------------------------------------- */