/* ---------------------------------------------------------------------- *
 * flipgraph.cpp
 *
 * author: jerome dohrau
 * ---------------------------------------------------------------------- */

#include "flipgraph.hpp"
#include "triangulation.hpp"

#include <queue>
#include <map>
#include <utility>
#include <iostream>
#include <algorithm>

/* ---------------------------------------------------------------------- *
 * flip graph
 *
 * possible improvements
 *  - use vector<set<int>> as datastructure for graph
 *  - use indices.find() to check whether triangulation already exists and
 *    reuse pointer to get index if it exists
 *  - only flip edges that are different in the context of isomorphism
 * ---------------------------------------------------------------------- */
 
void compute_flip_graph(int n, std::vector<std::vector<int> >& graph) {
    graph.clear();
    int count = 0;

    std::queue<std::pair<Triangulation*, int> > queue;
    std::map<Code, int> indices;

    // build canonical triangulation on n vertices
    Triangulation* triangulation = new Triangulation(n);

    // add canonical triangulation
    int index = count++;
    indices[Code(*triangulation)] = 0;
    graph.push_back(std::vector<int>());
    queue.push(std::make_pair(triangulation, index));

    // explore flip graph using a bfs
    while (!queue.empty()) {
        // get current triangulation
        triangulation = queue.front().first;
        index = queue.front().second;
        queue.pop();

        // loop through neighboring triangulations
        int m = triangulation->size();
        for (int i = 0; i < m; ++i) {
            Edge* edge = triangulation->edge(i);
            if (triangulation->is_representative(edge) && triangulation->is_flippable(edge)) {
                triangulation->flip(edge);

                Code code(*triangulation);
                int other_index = 0;
                // todo: faster if using indices.find() and reuse pointer to position
                if (indices.count(code) == 0) {
                    // add newly discovered triangulation
                    other_index = count++;
                    indices[code] = other_index;
                    graph.push_back(std::vector<int>());
                    queue.push(std::make_pair(new Triangulation(*triangulation), other_index));
                } else {
                    // get index of triangulation
                    other_index = indices[code];
                }

                // add edge if not already present
                if (std::count(graph[index].begin(), graph[index].end(), other_index) == 0) {
                    graph[index].push_back(other_index);
                }

                // note: after two flips the edge and its twin are swapped
                triangulation->flip(edge);
            }
        }

        delete triangulation;
    }
}

/* ---------------------------------------------------------------------- *
 * write functions
 * ---------------------------------------------------------------------- */

void write_flip_graph(std::vector<std::vector<int> >& graph, std::ostream& output_stream) {
    int size = (int) graph.size();
    for (int i = 0; i < size; ++i) {
        int degree = (int) graph[i].size();
        output_stream << i << ":";
        for (int j = 0; j < degree; ++j) { output_stream << " " << graph[i][j]; }
        output_stream << std::endl;
    }
}

/* ---------------------------------------------------------------------- *
 * end of file
 * ---------------------------------------------------------------------- */