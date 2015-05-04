/* ---------------------------------------------------------------------- *
 * triangulation.hpp
 *
 * author: jerome dohrau
 * ---------------------------------------------------------------------- */

#ifndef __FGG_TRIANGULATION__
#define __FGG_TRIANGULATION__

// if this macro is defined then the degree of a vertex is stored as a member
// and tracked/updated. otherwise, it is computed upon request.
#define STORE_DEGREE

// if this macro is defined then the isomorphism code uses some heuristics.
// these heuristics need the degree of the vertices ...
#define HEURISTICS

#include <vector>
#include <iostream>

/* ---------------------------------------------------------------------- *
 * forward declarations
 * ---------------------------------------------------------------------- */

class Vertex;
class Halfedge;
class Triangulation;
class Code;

/* ---------------------------------------------------------------------- *
 * declaration of the vertex class
 * ---------------------------------------------------------------------- */

class Vertex {
    // the label
    int label_;

    // an outgoing halfedge
    Halfedge* halfedge_;

    #ifdef STORE_DEGREE
    // this vertex's degree
    int degree_;
    #endif

public:
    // sets this vertex's label to the specified label
    void set_label(int label);

    // makes the specified halfedge an outgoing halfedge
    void set_halfedge(Halfedge* halfedge_);

    #ifdef STORE_DEGREE
    // sets this vertex's degree to the specified value
    void set_degree(int degree);

    // increases this vertex's degree by one
    void increase_degree();

    // decreases this vertex's degree by one
    void decrease_degree();
    #endif

    // returns this vertex's label
    int label() const;

    // returns an outgoing halfedge
    Halfedge* halfedge() const;

    // returns this vertex's degree
    int degree() const;
 };

/* ---------------------------------------------------------------------- *
 * declaration of the halfedge class
 * ---------------------------------------------------------------------- */ 

class Halfedge {
public:
    // constructor that creates a halfedge with the specified id
    Halfedge(int id);

private:
    // the id
    int id_;

    // the target vertex
    Vertex* target_;

    // the twin halfedge
    Halfedge* twin_;

    // the previous halfedge of the incident face
    Halfedge* prev_;

    // the next halfedge of the incident face
    Halfedge* next_;

public:
    // sets the target vertex to the specified vertex
    void set_target(Vertex* vertex);

    // sets the twin halfedge to the specified halfedge
    void set_twin(Halfedge* halfedge);

    // sets the next halfedge to the specified halfedge
    void set_prev(Halfedge* halfedge);

    // sets the next halfedge to the specified halfedge
    void set_next(Halfedge* halfedge);

    // returns the id
    int id() const;

    // returns the target vertex
    Vertex* target() const;

    // returns the twin halfedge
    Halfedge* twin() const;

    // returns the previous halfedge of the incident face
    Halfedge* prev() const;

    // returns the next halfedge of the incident face
    Halfedge* next() const;
};

/* ---------------------------------------------------------------------- *
 * declaration of the triangulation class
 * ---------------------------------------------------------------------- */

class Triangulation {
public:
    // the vertex list type
    typedef std::vector<Vertex*> VertexList;

    // the edge list type
    typedef std::vector<Halfedge*> EdgeList;

    // constructor that builds a canonical triangulation with n vertices
    Triangulation(int n);

    // constructor that builds a triangulation from the specified code
    Triangulation(const Code& code);

    // constructor that copies the specified triangulation
    Triangulation(const Triangulation& triangulation);

    // destructor
    ~Triangulation();

private:
    // the list of vertices
    VertexList vertices_;

    // the list of edges
    EdgeList halfedges_;

    // creates and returns a new vertex
    Vertex* new_vertex();

    // creates and returns a new halfedge
    Halfedge* new_edge();

    // makes the two specified edges twins
    void make_twins(Halfedge* halfedge_a, Halfedge* halfedge_b);

    // makes the two specified edges consecutive
    void make_consecutive(Halfedge* halfedge_a, Halfedge* halfedge_b);

    // builds a triangle with the three specified edges
    void make_triangle(
            Halfedge* halfedge_a, Halfedge* halfedge_b, Halfedge* halfedge_c);

    // builds a triangle with the three specified edges and vertices
    void make_triangle(
            Halfedge* halfedge_a, Halfedge* halfedge_b, Halfedge* halfedge_c,
            Vertex* vertex_a, Vertex* vertex_b, Vertex* vertex_c);

    // performs an e3-expansion at the specified halfedge
    void expand_three(Halfedge* halfedge);

    // builds a canonical triangulation with n vertices
    void make_canonical(int n);

    // builds a triangulation form the specified code
    void build_from_code(const Code& code);

    // builds a copy of the specified triangulation
    void copy(const Triangulation& triangulation);

    // removes all vertices and edges from the triangulation
    void clear();

public:
    // returns the number of vertices
    int order() const;

    // returns the number of edges
    int size() const;

    // returns the i-th vertex
    Vertex* vertex(int i) const;

    // returns the i-th halfedge
    Halfedge* halfedge(int i) const;

    // returns the halfedge from the first specified vertex to the second
    Halfedge* halfedge(Vertex* vertex_a, Vertex* vertex_b) const;

    // returns whether the specified halfedge or its twin is representative
    bool is_representative(Halfedge* halfedge) const;

    // returns whether the specified halfedge is flippable or not
    bool is_flippable(Halfedge* halfedge) const;

    // flips the specified halfedge
    void flip(Halfedge* halfedge);

    // writes the triangualtion to the specified stream
    void write_to_stream(std::ostream& output_stream) const;
};

/* ---------------------------------------------------------------------- *
 * declaration of the code class
 * ---------------------------------------------------------------------- */

class Code {
public:
    // constructor that computes code from the specified triangulation
    Code(const Triangulation& triangulation);

    // constructor that computes code from specified triangulation starting
    // at the specified halfedge
    Code(const Triangulation& triangulation, Halfedge* halfedge);

    // constructor that copies the specified code
    Code(const Code& code);

    // destructor
    ~Code();

private:
    // the char array representing the actual code
    unsigned char* code_;

    // the length of this code
    int length_;

    // initializes this code for the specified triangulation
    void initialize(const Triangulation& triangulation);

    // updates the this code for the specified triangulation starting at
    // the specified halfedge and using the specified orientation.
    // the code is updated if the new code is lexicographically smaller
    void update(const Triangulation& triangulation, Halfedge* halfedge, bool clockwise);

    // computes the code for the specified triangulation
    void compute_code(const Triangulation& triangulation);

    // computes the code for the specified triangulation starting at the
    // specified halfedge
    void compute_code(const Triangulation& triangulation, Halfedge* halfedge);

public:
    // sets the i-th symbol of the code
    void set_symbol(int i, unsigned char symbol);

    // returns the i-th symbol of the code
    unsigned char symbol(int i) const;

    // returns the lenghh of the code
    int length() const;

    // returns whether the code is equal to the specified code or not
    bool operator ==(const Code& other) const;

    // returns whether the code is not equal to the specified code or not
    bool operator !=(const Code& other) const;

    // returns whether the code lexicographically smaller than the specified
    // code or not
    bool operator <(const Code& other) const;

    // returns whether the code is lexicographically smaller than or
    // equal to the specified code or not
    bool operator <=(const Code& other) const;

    // returns whether the code is lexicographically greater than the specified
    // code or not
    bool operator >(const Code& other) const;

    // returns whether the code is lexicographically greater than or
    // equal to the specified code or not
    bool operator >=(const Code& other) const;

    // writes the code to the specified stream
    void write_to_stream(std::ostream& output_stream) const;
};

/* ---------------------------------------------------------------------- *
 * debug function declaration
 * ---------------------------------------------------------------------- */

// checks whether the specified triangulation is as it should be
void check_triangulation(Triangulation& triangulation);

#endif

/* ---------------------------------------------------------------------- *
 * end of file
 * ---------------------------------------------------------------------- */