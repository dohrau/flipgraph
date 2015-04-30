/* ---------------------------------------------------------------------- *
 * generator.hpp
 *
 * author: jerome dohrau
 * ---------------------------------------------------------------------- */

#ifndef __FGG_FLIP_GRAPH__
#define __FGG_FLIP_GRAPH__

#include "triangulation.hpp"

#include <vector>

/* ---------------------------------------------------------------------- *
 * definition of the flip graph class
 * ---------------------------------------------------------------------- */

class Flip_graph {
public:
    typedef std::vector<std::vector<int> > Graph;
    typedef std::vector<Code> CodeList;

private:
    Graph graph_;
    CodeList codes_;

public:
    void compute(int n);

    const Graph& graph() const;

    const Code& code(int i) const;

    void write_to_stream(std::ostream& output_stream) const;  
};

#endif

/* ---------------------------------------------------------------------- *
 * end of file
 * ---------------------------------------------------------------------- */