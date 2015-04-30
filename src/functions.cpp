/* ---------------------------------------------------------------------- *
 * functions.cpp
 *
 * author: jerome dohrau
 * ---------------------------------------------------------------------- */

 #include "functions.hpp"

 #include <queue>

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

/* ---------------------------------------------------------------------- *
 * end of file
 * ---------------------------------------------------------------------- */