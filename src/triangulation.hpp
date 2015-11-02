/* ---------------------------------------------------------------------- *
 * triangulation.hpp
 * ---------------------------------------------------------------------- */

#ifndef __FGG_TRIANGULATION__
#define __FGG_TRIANGULATION__

#include <vector>
#include <iostream>

/* ---------------------------------------------------------------------- *
 * constants
 * ---------------------------------------------------------------------- */

const int TRIANGULATION_CANONICAL = 1;
const int TRIANGULATION_DOMINANT_ZIG_ZAG = 2;
const int TRIANGULATION_DOMINANT_BINARY_TREE = 3;

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
    Halfedge *halfedge_;

    // this vertex's degree
    int degree_;

public:
    // sets this vertex's label to the specified label
    void set_label(int label);

    // makes the specified halfedge an outgoing halfedge
    void set_halfedge(Halfedge *halfedge_);

    // sets this vertex's degree to the specified value
    void set_degree(int degree);

    // increases this vertex's degree by one
    void increase_degree();

    // decreases this vertex's degree by one
    void decrease_degree();

    // returns this vertex's label
    int label() const;

    // returns an outgoing halfedge
    Halfedge *halfedge() const;

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
    Vertex *target_;

    // the twin halfedge
    Halfedge *twin_;

    // the previous halfedge of the incident face
    Halfedge *prev_;

    // the next halfedge of the incident face
    Halfedge *next_;

public:
    // sets the id
    void set_id(int id);

    // sets the target vertex to the specified vertex
    void set_target(Vertex *vertex);

    // sets the twin halfedge to the specified halfedge
    void set_twin(Halfedge *halfedge);

    // sets the next halfedge to the specified halfedge
    void set_prev(Halfedge *halfedge);

    // sets the next halfedge to the specified halfedge
    void set_next(Halfedge *halfedge);

    // returns the id
    int id() const;

    // returns the target vertex
    Vertex *target() const;

    // returns the twin halfedge
    Halfedge *twin() const;

    // returns the previous halfedge of the incident face
    Halfedge *prev() const;

    // returns the next halfedge of the incident face
    Halfedge *next() const;
};

/* ---------------------------------------------------------------------- *
 * declaration of the triangulation class
 * ---------------------------------------------------------------------- */

class Triangulation {
public:
    // the vertex list type
    typedef std::vector<Vertex *> VertexList;

    // the edge list type
    typedef std::vector<Halfedge *> EdgeList;

    // constructor that builds triangulation with n vertices.
    // the second argument determines the structure of the triangulation.
    // if only one argument is given the canonical triangulation is built.
    Triangulation(int n, int triangulation_type = TRIANGULATION_CANONICAL);

    // constructor that builds a triangulation from the specified code
    Triangulation(const Code &code);

    // constructor that copies the specified triangulation
    Triangulation(const Triangulation &triangulation);

    // destructor
    ~Triangulation();

protected:
    // the list of vertices
    VertexList vertices_;

    // the list of edges
    EdgeList halfedges_;

    // creates and returns a new vertex
    Vertex *new_vertex();

    // creates and returns a new halfedge
    Halfedge *new_edge();

    // makes the two specified halfedges twins
    void make_twins(Halfedge *halfedge_a, Halfedge *halfedge_b);

    // makes the halfedge (and its twin) fixed, i.e., unflippable
    void make_fixed(Halfedge*halfedge);

    // makes the two specified edges consecutive
    void make_consecutive(Halfedge *halfedge_a, Halfedge *halfedge_b);

    // builds a triangle with the three specified edges
    void make_triangle(
            Halfedge *halfedge_a, Halfedge *halfedge_b, Halfedge *halfedge_c);

    // builds a triangle with the three specified edges and vertices
    void make_triangle(
            Halfedge *halfedge_a, Halfedge *halfedge_b, Halfedge *halfedge_c,
            Vertex *vertex_a, Vertex *vertex_b, Vertex *vertex_c);

    // performs an e3-expansion at the specified halfedge
    void edpand(Halfedge *halfedge);

    // splits the specified halfedge and the face incident to it
    void split(Halfedge* halfedge);

    // builds the first triangle of the triangulation
    void build_first_triangle();

    // builds a canonical triangulation with n vertices
    void build_canonical(int n);

    // builds a triangulation with one dominant vertex where the rest forms a bistar
    void build_dominant_zig_zag(int n);

    // builds a triangualtion with one dominant vertex where the rest forms a binary tree
    void build_dominant_binary_tree(int n);

    // builds a canonical outerplanar triangulation
    void build_outerplanar(int n);

    // builds a triangulation form the specified code
    void build_from_code(const Code &code);

    // builds a copy of the specified triangulation
    void copy(const Triangulation &triangulation);

    // removes all vertices and edges from the triangulation
    void clear();

public:
    // returns the number of vertices
    int order() const;

    // returns the number of edges
    int size() const;

    // returns the minimal degree among all vertices
    int min_degree() const;

    // returns the maximal degree among all vertices
    int max_degree() const;

    // returns whether the triangulation has a dominant vertex or not
    bool has_dominant_vertex() const;

    // returns the i-th vertex
    Vertex *vertex(int i) const;

    // returns the i-th halfedge
    Halfedge *halfedge(int i) const;

    // returns the halfedge from the first specified vertex to the second
    Halfedge *halfedge(Vertex *vertex_a, Vertex *vertex_b) const;

    // returns whether the specified halfedge or its twin is representative
    bool is_representative(Halfedge *halfedge) const;

    // returns whether the specified halfedge is flippable or not
    bool is_flippable(Halfedge *halfedge) const;

    // flips the specified halfedge
    void flip(Halfedge *halfedge);

    // writes the triangualtion to the specified stream
    void write_to_stream(std::ostream &output_stream) const;

protected:
    // debug
    void check(Triangulation &triangulation);
};

/* ---------------------------------------------------------------------- *
 * declaration of the code class
 * ---------------------------------------------------------------------- */

class Code {
public:
    // constructor that computes code from the specified triangulation
    Code(const Triangulation &triangulation);

    // constructor that computes code from specified triangulation starting
    // at the specified halfedge
    Code(const Triangulation &triangulation, Halfedge *halfedge);

    // constructor that copies the specified code
    Code(const Code &code);

    // constructor that copies the code from the specified vector
    Code(const std::vector<unsigned char> &code);

    // destructor
    ~Code();

private:
    // the char array representing the actual code
    unsigned char *code_;

    // the length of this code
    int length_;

    // initializes this code for the specified triangulation
    void initialize(const Triangulation &triangulation);

    // updates the this code for the specified triangulation starting at
    // the specified halfedge and using the specified orientation.
    // the code is updated if the new code is lexicographically smaller
    void update(const Triangulation &triangulation, Halfedge *halfedge, bool clockwise);

    // computes the code for the specified triangulation
    void compute_code(const Triangulation &triangulation);

    // computes the code for the specified triangulation starting at the
    // specified halfedge
    void compute_code(const Triangulation &triangulation, Halfedge *halfedge);

public:
    // sets the i-th symbol of the code
    void set_symbol(int i, unsigned char symbol);

    // returns the i-th symbol of the code
    unsigned char symbol(int i) const;

    // returns the length of the code
    int length() const;

    // returns whether the code is equal to the specified code or not
    bool operator==(const Code &other) const;

    // returns whether the code is not equal to the specified code or not
    bool operator!=(const Code &other) const;

    // returns whether the code lexicographically smaller than the specified
    // code or not
    bool operator<(const Code &other) const;

    // returns whether the code is lexicographically smaller than or
    // equal to the specified code or not
    bool operator<=(const Code &other) const;

    // returns whether the code is lexicographically greater than the specified
    // code or not
    bool operator>(const Code &other) const;

    // returns whether the code is lexicographically greater than or
    // equal to the specified code or not
    bool operator>=(const Code &other) const;

    // writes the code to the specified stream
    void write_to_stream(std::ostream &output_stream) const;
};

#endif

/* ---------------------------------------------------------------------- *
 * end of file
 * ---------------------------------------------------------------------- */