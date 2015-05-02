/* ---------------------------------------------------------------------- *
 * functions.cpp
 *
 * author: jerome dohrau
 * ---------------------------------------------------------------------- */

 #include "functions.hpp"

 #include <queue>
 #include <utility>

 int vertex_eccentricity(const Graph& graph, int vertex) {
    int n = (int) graph.size();
    int result = 0;

    std::vector<bool> visited(n, false);
    std::queue<std::pair<int, int> > queue;

    visited[vertex] = true;
    queue.push(std::make_pair(vertex, 0));

    while (!queue.empty()) {
        int index = queue.front().first;
        int distance = queue.front().second;
        queue.pop();

        result = std::max(result, distance);

        int degree = (int) graph[index].size();
        for (int i = 0; i < degree; ++i) {
            int neighbor = graph[index][i];
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                queue.push(std::make_pair(neighbor, distance + 1));
            }
        }
    }

    return result;
}

int graph_diameter(const Graph& graph) {
    int n = (int) graph.size();
    int result = 0;

    for (int i = 0; i < n; ++i) {
        int eccentricity = vertex_eccentricity(graph, i);
        result = std::max(result, eccentricity);
    }

    return result;
}

void distance_histogram(const Graph& graph, std::vector<int>& vertices, std::vector<int>& histogram) {
    int n = (int) graph.size();
    int l = (int) vertices.size();
    int maximum = 0;

    std::vector<int> distances(n, -1);
    std::queue<std::pair<int, int> > queue;

    for (int i = 0; i < l; ++i) {
        int vertex = vertices[i];
        if (distances[vertex] == -1) {
            distances[vertex] = 0;
            queue.push(std::make_pair(vertex, 0));
        }
    }

    while (!queue.empty()) {
        int index = queue.front().first;
        int distance = queue.front().second;
        queue.pop();

        maximum = std::max(maximum, distance);

        int degree = (int) graph[index].size();
        for (int i = 0; i < degree; ++i) {
            int neighbor = graph[index][i];
            if (distances[neighbor] == -1) {
                distances[neighbor] = distance + 1;
                queue.push(std::make_pair(neighbor, distance + 1));
            }
        }
    }

    histogram.clear();
    histogram.resize(maximum + 1, 0);
    for (int i = 0; i < n; ++i) { histogram[distances[i]]++; }
}

/* ---------------------------------------------------------------------- *
 * end of file
 * ---------------------------------------------------------------------- */