/* ---------------------------------------------------------------------- *
 * generator.cpp
 * ---------------------------------------------------------------------- */

#include "flipgraph.hpp"
#include "triangulation.hpp"

#include <queue>
#include <map>
#include <set>
#include <utility>
#include <iostream>
#include <algorithm>

typedef FlipGraph::Graph Graph;

/* ---------------------------------------------------------------------- *
 * possible improvements
 *  - use vector<set<int>> as data structure for graph
 *  - use indices.find() to check whether triangulation already exists and
 *    reuse pointer to get index if it exists
 *  - only flip edges that are different in the context of isomorphism
 * ---------------------------------------------------------------------- */

void FlipGraph::compute(int n) {
    graph_.clear();
    int count = 0;

    std::queue<std::pair<Triangulation *, int> > queue;
    std::map<Code, int> indices;

    // build canonical triangulation on n vertices
    Triangulation *triangulation = new Triangulation(n);
    Code *code = new Code(*triangulation);

    // add canonical triangulation
    int index = count++;
    indices[*code] = 0;
    graph_.push_back(std::vector<int>());
    codes_.push_back(*code);
    queue.push(std::make_pair(triangulation, index));

    delete code;

    // explore flip graph_ using a bfs
    while (!queue.empty()) {
        // get current triangulation
        triangulation = queue.front().first;
        index = queue.front().second;
        queue.pop();

        // loop through neighboring triangulations
        int m = triangulation->size();
        //std::set<Code> edge_codes;
        for (int i = 0; i < m; ++i) {
            Halfedge *halfedge = triangulation->halfedge(i);
            if (triangulation->is_representative(halfedge) && triangulation->is_flippable(halfedge)) {
                //Code edge_code(*triangulation, halfedge);
                //if (edge_codes.find(edge_code) != edge_codes.end()) { continue; }
                //edge_codes.insert(edge_code);

                triangulation->flip(halfedge);

                Code triangulation_code(*triangulation);
                int other_index = 0;
                // todo: faster if using indices.find() and reuse pointer to position
                if (indices.count(triangulation_code) == 0) {
                    // add newly discovered triangulation
                    other_index = count++;
                    indices[triangulation_code] = other_index;
                    graph_.push_back(std::vector<int>());
                    codes_.push_back(triangulation_code);
                    queue.push(std::make_pair(new Triangulation(*triangulation), other_index));
                } else {
                    // get index of triangulation
                    other_index = indices[triangulation_code];
                }

                // add halfedge if not already present
                if (std::count(graph_[index].begin(), graph_[index].end(), other_index) == 0) {
                    graph_[index].push_back(other_index);
                }

                // note: after two flips the halfedge and its twin are swapped
                // all other edges stay in place. this is crucial since
                // we loop over all edges.
                triangulation->flip(halfedge);

            }
        }

        delete triangulation;
    }


}

const Graph &FlipGraph::graph() const {
    return graph_;
}

const Code &FlipGraph::code(int i) const {
    return codes_[i];
}

void FlipGraph::write_to_stream(std::ostream &output_stream) const {
    int size = (int) graph_.size();
    output_stream << size << std::endl;
    for (int i = 0; i < size; ++i) {
        int degree = (int) graph_[i].size();
        output_stream << i;
        for (int j = 0; j < degree; ++j) { output_stream << " " << graph_[i][j]; }
        output_stream << std::endl;
    }
}

/* ---------------------------------------------------------------------- *
 * end of file
 * ---------------------------------------------------------------------- */