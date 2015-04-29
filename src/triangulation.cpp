/* ---------------------------------------------------------------------- *
 * triangulation.cpp
 *
 * author: jerome dohrau
 * ---------------------------------------------------------------------- */

// #define NDEBUG

#include "triangulation.hpp"

#include <queue>
#include <map>
#include <cassert>

/* ---------------------------------------------------------------------- *
 * implementation of the vertex class
 * ---------------------------------------------------------------------- */

void Vertex::set_label(int label) {
    label_ = label;
}

void Vertex::set_halfedge(Halfedge* halfedge) {
    halfedge_ = halfedge;
}

int Vertex::label() const {
    return label_;
}

Halfedge* Vertex::halfedge() const {
    return halfedge_;
}

int Vertex::degree() const {
    int degree = 0;
    Halfedge* current = halfedge_;
    do {
        degree++;
        current = current->twin()->next();
    } while (current != halfedge_);
    return degree;
}

/* ---------------------------------------------------------------------- *
 * implementation of the halfedge class
 * ---------------------------------------------------------------------- */

void Halfedge::set_target(Vertex* vertex) {
    target_ = vertex;
}

void Halfedge::set_twin(Halfedge* halfedge) {
    twin_ = halfedge;
}

void Halfedge::set_prev(Halfedge* halfedge) {
    prev_ = halfedge;
}

void Halfedge::set_next(Halfedge* halfedge) {
    next_ = halfedge;
}

Vertex* Halfedge::target() const {
    return target_;
}

Halfedge* Halfedge::twin() const {
    return twin_;
}

Halfedge* Halfedge::prev() const {
    return prev_;
}

Halfedge* Halfedge::next() const {
    return next_;
}

/* ---------------------------------------------------------------------- *
 * implementation of the triangulation class
 * ---------------------------------------------------------------------- */

Triangulation::Triangulation(int n) {
    make_canonical(n);
}

Triangulation::Triangulation(const Code& code) {
    build_from_code(code);
}

Triangulation::Triangulation(const Triangulation& triangulation) {
    copy(triangulation);
}

Triangulation::~Triangulation() {
    clear();
}

Vertex* Triangulation::new_vertex() {
    Vertex* vertex = new Vertex();
    vertices_.push_back(vertex);
    return vertex;
}

Halfedge* Triangulation::new_edge() {
    Halfedge* halfedge = new Halfedge();
    edges_.push_back(halfedge);
    return halfedge;
}

void Triangulation::make_twins(Halfedge* halfedge_a, Halfedge* halfedge_b) {
    halfedge_a->set_twin(halfedge_b);
    halfedge_b->set_twin(halfedge_a);
}

void Triangulation::make_consecutive(Halfedge* halfedge_a, Halfedge* halfedge_b) {
    halfedge_a->set_next(halfedge_b);
    halfedge_b->set_prev(halfedge_a);
}

void Triangulation::make_triangle(
        Halfedge* halfedge_a, Halfedge* halfedge_b, Halfedge* halfedge_c) {
    make_consecutive(halfedge_a, halfedge_b);
    make_consecutive(halfedge_b, halfedge_c);
    make_consecutive(halfedge_c, halfedge_a);
}

void Triangulation::make_triangle(
        Halfedge* halfedge_a, Halfedge* halfedge_b, Halfedge* halfedge_c,
        Vertex* vertex_a, Vertex* vertex_b, Vertex* vertex_c) {
    make_triangle(halfedge_a, halfedge_b, halfedge_c);
    vertex_a->set_halfedge(halfedge_c);
    vertex_b->set_halfedge(halfedge_a);
    vertex_c->set_halfedge(halfedge_b);
    halfedge_a->set_target(vertex_c);
    halfedge_b->set_target(vertex_a);
    halfedge_c->set_target(vertex_b);
}

void Triangulation::expand_three(Halfedge* halfedge) {
    Halfedge* halfedge_ab = halfedge;
    Halfedge* halfedge_bc = halfedge->next();
    Halfedge* halfedge_ca = halfedge->prev();
    Vertex* vertex_a = halfedge_ca->target();
    Vertex* vertex_b = halfedge_ab->target();
    Vertex* vertex_c = halfedge_bc->target();
    Vertex* vertex_d = new_vertex();
    
    make_triangle(halfedge_ab, new_edge(), new_edge(), vertex_d, vertex_a, vertex_b);
    make_triangle(halfedge_bc, new_edge(), new_edge(), vertex_d, vertex_b, vertex_c);
    make_triangle(halfedge_ca, new_edge(), new_edge(), vertex_d, vertex_c, vertex_a);
    make_twins(halfedge_ab->next(), halfedge_bc->prev());
    make_twins(halfedge_bc->next(), halfedge_ca->prev());
    make_twins(halfedge_ca->next(), halfedge_ab->prev());
}

void Triangulation::make_canonical(int n) {
    Vertex* vertex_a = new_vertex();
    Vertex* vertex_b = new_vertex();
    Vertex* vertex_c = new_vertex();
    Halfedge* halfedge_ab = new_edge();
    Halfedge* halfedge_bc = new_edge();
    Halfedge* halfedge_ca = new_edge();
    Halfedge* halfedge_ba = new_edge();
    Halfedge* halfedge_cb = new_edge();
    Halfedge* halfedge_ac = new_edge();
    
    // build first triangle
    make_triangle(halfedge_ab, halfedge_bc, halfedge_ca, vertex_c, vertex_a, vertex_b);
    make_triangle(halfedge_ac, halfedge_cb, halfedge_ba, vertex_b, vertex_a, vertex_c);
    make_twins(halfedge_ab, halfedge_ba);
    make_twins(halfedge_bc, halfedge_cb);
    make_twins(halfedge_ca, halfedge_ac);
    
    // apply e3-expansion n-3 times
    for (int i = 3; i < n; ++i) {
        expand_three(halfedge_ab);
    }

    #ifndef NDEBUG
    check_triangulation(*this);
    #endif
}

void Triangulation::build_from_code(const Code& code) {
    int l = code.length();
    int n = (int) code.symbol(0);
    
    for (int i = 0; i < n; ++i) { new_vertex(); }
    
    int index = 0;
    for (int i = 0; i < n; ++i) {
        Vertex* vertex_a = vertex(i);
        
        Halfedge* first = nullptr;
        Halfedge* last = nullptr;
        
        while (code.symbol(++index)) {
            int j = (int) (code.symbol(index) - 1);
            Vertex* vertex_b = vertex(j);
            
            Halfedge* current;
            if (i < j) {
                // create new halfedge
                current = new_edge();
                current->set_target(vertex_b);
                make_twins(current, new_edge());
                current->twin()->set_target(vertex_a);
            } else {
                // get existing halfedge
                current = halfedge(vertex_b, vertex_a);
                assert(current != nullptr);
                current = current->twin();
            }
            
            if (first == nullptr) { first = current; }
            if (last != nullptr) { make_consecutive(last, current); }
            last = current->twin();
        }
        
        make_consecutive(last, first);
        vertex_a->set_halfedge(first);
    }

    #ifndef NDEBUG
    check_triangulation(*this);
    #endif
}

void Triangulation::copy(const Triangulation& triangulation) {
    int n = triangulation.order();
    int m = triangulation.size();
    std::map<Vertex*, Vertex*> vertex_map;
    std::map<Halfedge*, Halfedge*> edge_map;
    
    for (int i = 0; i < n; ++i) {
        Vertex* vertex = triangulation.vertex(i);
        Vertex* copy = new_vertex();
        vertex_map[vertex] = copy;
    }
    
    for (int i = 0; i < m; ++i) {
        Halfedge* halfedge = triangulation.halfedge(i);
        Halfedge* copy = new_edge();
        edge_map[halfedge] = copy;
    }
    
    for (int i = 0; i < n; ++i) {
        Vertex* vertex = triangulation.vertex(i);
        Vertex* copy = vertex_map[vertex];
        copy->set_label(vertex->label());
        copy->set_halfedge(edge_map[vertex->halfedge()]);
    }
    
    for (int i = 0; i < m; ++i) {
        Halfedge* halfedge = triangulation.halfedge(i);
        Halfedge* copy = edge_map[halfedge];
        copy->set_target(vertex_map[halfedge->target()]);
        copy->set_twin(edge_map[halfedge->twin()]);
        copy->set_prev(edge_map[halfedge->prev()]);
        copy->set_next(edge_map[halfedge->next()]);
    }

    #ifndef NDEBUG
    check_triangulation(*this);
    #endif
}

void Triangulation::clear() {
    int n = (int) vertices_.size();;
    int m = (int) edges_.size();
    for (int i = 0; i < n; ++i) { delete vertices_[i]; }
    for (int j = 0; j < m; ++j) { delete edges_[j]; }
    edges_.clear();
    vertices_.clear();
}

int Triangulation::order() const {
    return (int) vertices_.size();
}

int Triangulation::size() const {
    return (int) edges_.size();
}

Vertex* Triangulation::vertex(int i) const {
    return vertices_[i];
}

Halfedge* Triangulation::halfedge(int i) const {
    return edges_[i];
}

Halfedge* Triangulation::halfedge(Vertex* vertex_a, Vertex* vertex_b) const {
    Halfedge* first = vertex_a->halfedge();
    Halfedge* current = first;
    do {
        if (current->target() == vertex_b) { return current; }
        current = current->twin()->next();
    } while (current != first);
    return nullptr;
}

bool Triangulation::is_representative(Halfedge* halfedge) {
    return halfedge < halfedge->twin();
}

bool Triangulation::is_flippable(Halfedge* halfedge) {
    Vertex* vertex_a = halfedge->next()->target();
    Vertex* vertex_b = halfedge->twin()->next()->target();
    Halfedge* first = vertex_a->halfedge();
    Halfedge* current = first;
    
    // check whether vertex_a and vertex_b are adjacent
    do {
        if (current->target() == vertex_b) { return false; }
        current = current->twin()->next();
    } while (current != first);
    return true;
}

void Triangulation::flip(Halfedge* halfedge) {
    assert(is_flippable(halfedge));

    Halfedge* twin = halfedge->twin();
    Vertex* vertex_t = halfedge->target();
    Vertex* vertex_s =twin->target();
    Vertex* vertex_a = halfedge->next()->target();
    Vertex* vertex_b = twin->next()->target();
    Halfedge* edge_ta = halfedge->next();
    Halfedge* edge_as = halfedge->prev();
    Halfedge* edge_sb = twin->next();
    Halfedge* edge_bt = twin->prev();
    
    make_triangle(halfedge, edge_bt, edge_ta);
    make_triangle(twin, edge_as, edge_sb);
    halfedge->set_target(vertex_b);
    twin->set_target(vertex_a);
    if (vertex_t->halfedge() == twin) { vertex_t->set_halfedge(edge_ta); }
    if (vertex_s->halfedge() == halfedge) { vertex_s->set_halfedge(edge_sb); }
}

/* ---------------------------------------------------------------------- *
 * impementation of the code class
 * ---------------------------------------------------------------------- */

Code::Code(const Triangulation& triangulation) {
    compute_code(triangulation);
}

Code::Code(const Triangulation& triangulation, Halfedge* halfedge) {
    compute_code(triangulation, halfedge);
}

Code::Code(const Code& code) {
    length_ = code.length_;
    code_ = new unsigned char[length_];
    for (int i = 0; i < length_; ++i) { code_[i] = code.symbol(i); }
}

Code::~Code() {
    delete[] code_;
}

void Code::initialize(const Triangulation& triangulation) {
    int n = triangulation.order();
    int m = triangulation.size();
    length_ = n + m + 1;
    code_ = new unsigned char[length_];
    for (int i = 0; i < length_; ++i) { code_[i] = n; }
}

void Code::update(const Triangulation& triangulation, Halfedge* halfedge, bool clockwise) {
    int n = triangulation.order();
    for (int i = 0; i < n; ++i) {
        Vertex* vertex = triangulation.vertex(i);
        vertex->set_label(0);
    }

    int index = 1;
    int label = 1;
    bool smaller = false;
    std::queue<Halfedge*> queue;

    // breadth first search
    halfedge->target()->set_label(label++);
    queue.push(halfedge);
    while (!queue.empty()) {
        Halfedge* first = queue.front()->twin();
        Halfedge* current = first;
        queue.pop();

        do {
            Vertex* vertex = current->target();
            if (vertex->label() == 0) {
                vertex->set_label(label++);
                queue.push(current);
            }

            int symbol = vertex->label();
            if (smaller) {
                code_[index] = symbol;
            } else if (symbol < code_[index]) {
                smaller = true;
                code_[index] = symbol;
            } else if (symbol > code_[index]) {
                return;
            }

            index++;
            current = clockwise ? current->twin()->next() : current->prev()->twin();
        } while (current != first);

        // mark end of current vertex's phase
        if (code_[index] > 0) { smaller = true; }
        code_[index++] = 0;
    }
}

void Code::compute_code(const Triangulation& triangulation) {
    initialize(triangulation);
    int m = triangulation.size();
    for (int i = 0; i < m; ++i) {
        Halfedge* halfedge = triangulation.halfedge(i);
        update(triangulation, halfedge, true);
        update(triangulation, halfedge, false);
    }
}

void Code::compute_code(const Triangulation& triangulation, Halfedge* halfedge) {
    initialize(triangulation);
    Halfedge* twin = halfedge->twin();
    update(triangulation, halfedge, true);
    update(triangulation, halfedge, false);
    update(triangulation, twin, true);
    update(triangulation, twin, false);
}

void Code::set_symbol(int i, unsigned char symbol) {
    code_[i] = symbol;
}

unsigned char Code::symbol(int i) const {
    return code_[i];
}

int Code::length() const {
    return length_;
}

bool Code::operator ==(const Code& other) const {
    if (length_ != other.length_) { return false; }
    for (int i = 0; i < length_; ++i) {
        if (code_[i] != other.code_[i]) { return false; }
    }
    return true;
}

bool Code::operator !=(const Code& other) const {
    return !(*this == other);
}

bool Code::operator <(const Code& other) const {
    if (length_ < other.length_) { return true; }
    if (length_ > other.length_) { return false; }
    for (int i = 0; i < length_; ++i) {
        if (code_[i] < other.code_[i]) { return true; }
        if (code_[i] > other.code_[i]) { return false; }
    }
    return false;
}

bool Code::operator <=(const Code& other) const {
    if (length_ < other.length_) { return true; }
    if (length_ > other.length_) { return false; }
    for (int i = 0; i < length_; ++i) {
        if (code_[i] < other.code_[i]) { return true; }
        if (code_[i] > other.code_[i]) { return false; }
    }
    return true;
}

bool Code::operator >(const Code& other) const {
    return other < *this;
}

bool Code::operator >=(const Code& other) const {
    return other <= *this;
}

/* ---------------------------------------------------------------------- *
 * write functions implementation
 * ---------------------------------------------------------------------- */

/* ---------------------------------------------------------------------- *
 * debug functions implementation
 * ---------------------------------------------------------------------- */

void check_triangulation(Triangulation& triangulation) {
    int n = triangulation.order();
    int m = triangulation.size();
    assert(m == 2*(3*n-6));

    // check vertex pointers
    for (int i = 0; i < n; ++i) {
        Vertex* vertex = triangulation.vertex(i);
        assert(vertex == vertex->halfedge()->twin()->target());
    }

    // check halfedge pointers
    for (int i = 0; i < m; ++i) {
        Halfedge* halfedge = triangulation.halfedge(i);
        assert(halfedge == halfedge->twin()->twin());
        assert(halfedge == halfedge->prev()->next());
        assert(halfedge == halfedge->next()->prev());
        assert(halfedge == halfedge->next()->next()->next());
        assert(halfedge == halfedge->prev()->prev()->prev());
        assert(halfedge->next() == halfedge->prev()->prev());
        assert(halfedge->prev() == halfedge->next()->next());
        assert(halfedge->target() == halfedge->next()->twin()->target());
    }

    // handshaking lemma
    int sum = 0;
    for (int i = 0; i < n; ++i) {
        Vertex* vertex = triangulation.vertex(i);
        int degree = vertex->degree();
        assert(degree >= 3);
        sum += degree;
    }
    assert(sum == m);
}

/* ---------------------------------------------------------------------- *
 * end of file
 * ---------------------------------------------------------------------- */