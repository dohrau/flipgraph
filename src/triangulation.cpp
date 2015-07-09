/* ---------------------------------------------------------------------- *
 * triangulation.cpp
 * ---------------------------------------------------------------------- */

#define OUTERPLANAR
//#define NDEBUG

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

void Vertex::set_halfedge(Halfedge *halfedge) {
    halfedge_ = halfedge;
}

void Vertex::set_degree(int degree) {
    degree_ = degree;
}

void Vertex::increase_degree() {
    degree_++;
}

void Vertex::decrease_degree() {
    degree_--;
}

int Vertex::label() const {
    return label_;
}

Halfedge *Vertex::halfedge() const {
    return halfedge_;
}

int Vertex::degree() const {
    return degree_;
}

/* ---------------------------------------------------------------------- *
 * implementation of the halfedge class
 * ---------------------------------------------------------------------- */

Halfedge::Halfedge(int id) : id_(id) {
    target_ = nullptr; // TODO: remove
}

void Halfedge::set_id(int id) {
    id_ = id;
}

void Halfedge::set_target(Vertex *vertex) {
    target_ = vertex;
}

void Halfedge::set_twin(Halfedge *halfedge) {
    twin_ = halfedge;
}

void Halfedge::set_prev(Halfedge *halfedge) {
    prev_ = halfedge;
}

void Halfedge::set_next(Halfedge *halfedge) {
    next_ = halfedge;
}

int Halfedge::id() const {
    return id_;
}

Vertex *Halfedge::target() const {
    return target_;
}

Halfedge *Halfedge::twin() const {
    return twin_;
}

Halfedge *Halfedge::prev() const {
    return prev_;
}

Halfedge *Halfedge::next() const {
    return next_;
}

/* ---------------------------------------------------------------------- *
 * implementation of the triangulation class
 * ---------------------------------------------------------------------- */

Triangulation::Triangulation(int n, int triangulation_type) {
#ifdef OUTERPLANAR
    build_outerplanar(n);
#else
    switch (triangulation_type) {
        case TRIANGULATION_CANONICAL:
            build_canonical(n);
            break;
        case TRIANGULATION_DOMINANT_ZIG_ZAG:
            build_dominant_zig_zag(n);
            break;
        case TRIANGULATION_DOMINANT_TWIN_STAR:
            build_dominant_twin_star(n);
            break;
        case TRIANGULATION_DOMINANT_BINARY_TREE:
            build_dominant_binary_tree(n);
            break;
        default:
            build_canonical(n);
    }
#endif
}

Triangulation::Triangulation(const Code &code) {
    build_from_code(code);
}

Triangulation::Triangulation(const Triangulation &triangulation) {
    copy(triangulation);
}

Triangulation::~Triangulation() {
    clear();
}

Vertex *Triangulation::new_vertex() {
    Vertex *vertex = new Vertex();
    vertices_.push_back(vertex);
    return vertex;
}

Halfedge *Triangulation::new_edge() {
    int id = halfedges_.size();
    Halfedge *halfedge = new Halfedge(id);
    halfedges_.push_back(halfedge);
    return halfedge;
}

void Triangulation::make_twins(Halfedge *halfedge_a, Halfedge *halfedge_b) {
    halfedge_a->set_twin(halfedge_b);
    halfedge_b->set_twin(halfedge_a);
}

void Triangulation::make_fixed(Halfedge *halfedge) {
    // by giving the halfedge and its twin the same id, none of
    // them is the representative edge anymore.
    halfedge->set_id(halfedge->twin()->id());
}

void Triangulation::make_consecutive(Halfedge *halfedge_a, Halfedge *halfedge_b) {
    halfedge_a->set_next(halfedge_b);
    halfedge_b->set_prev(halfedge_a);
}

void Triangulation::make_triangle(
        Halfedge *halfedge_a, Halfedge *halfedge_b, Halfedge *halfedge_c) {
    make_consecutive(halfedge_a, halfedge_b);
    make_consecutive(halfedge_b, halfedge_c);
    make_consecutive(halfedge_c, halfedge_a);
}

void Triangulation::make_triangle(
        Halfedge *halfedge_a, Halfedge *halfedge_b, Halfedge *halfedge_c,
        Vertex *vertex_a, Vertex *vertex_b, Vertex *vertex_c) {
    make_triangle(halfedge_a, halfedge_b, halfedge_c);
    vertex_a->set_halfedge(halfedge_c);
    vertex_b->set_halfedge(halfedge_a);
    vertex_c->set_halfedge(halfedge_b);
    halfedge_a->set_target(vertex_c);
    halfedge_b->set_target(vertex_a);
    halfedge_c->set_target(vertex_b);
}

void Triangulation::edpand(Halfedge *halfedge) {
    Halfedge *halfedge_ab = halfedge;
    Halfedge *halfedge_bc = halfedge->next();
    Halfedge *halfedge_ca = halfedge->prev();
    Vertex *vertex_a = halfedge_ca->target();
    Vertex *vertex_b = halfedge_ab->target();
    Vertex *vertex_c = halfedge_bc->target();
    Vertex *vertex_d = new_vertex();

    // perform expansion
    make_triangle(halfedge_ab, new_edge(), new_edge(), vertex_d, vertex_a, vertex_b);
    make_triangle(halfedge_bc, new_edge(), new_edge(), vertex_d, vertex_b, vertex_c);
    make_triangle(halfedge_ca, new_edge(), new_edge(), vertex_d, vertex_c, vertex_a);
    make_twins(halfedge_ab->next(), halfedge_bc->prev());
    make_twins(halfedge_bc->next(), halfedge_ca->prev());
    make_twins(halfedge_ca->next(), halfedge_ab->prev());

    // adjust degrees
    vertex_a->increase_degree();
    vertex_b->increase_degree();
    vertex_c->increase_degree();
    vertex_d->set_degree(3);
}

void Triangulation::split(Halfedge *halfedge) {
    Halfedge *halfedge_am = halfedge;
    Halfedge *halfedge_ma = halfedge_am->twin();
    Halfedge *halfedge_bm = new_edge();
    Halfedge *halfedge_mb = new_edge();
    Halfedge *halfedge_bc = halfedge->next();
    Halfedge *halfedge_ca = halfedge->prev();
    Halfedge *halfedge_cm = new_edge();
    Halfedge *halfedge_mc = new_edge();
    Vertex *vertex_a = halfedge_ca->target();
    Vertex *vertex_b = halfedge_am->target();
    Vertex *vertex_c = halfedge_bc->target();
    Vertex *vertex_m = new_vertex();

    // perform split
    make_triangle(halfedge_am, halfedge_mc, halfedge_ca, vertex_c, vertex_a, vertex_m);
    make_triangle(halfedge_mb, halfedge_bc, halfedge_cm, vertex_c, vertex_m, vertex_b);
    make_twins(halfedge_bm, halfedge_mb);
    make_twins(halfedge_cm, halfedge_mc);
    make_consecutive(halfedge_ma->prev(), halfedge_bm);
    make_consecutive(halfedge_bm, halfedge_ma);
    make_fixed(halfedge_bm);
    halfedge_bm->set_target(vertex_m);

    // adjust degrees
    vertex_c->increase_degree();
    vertex_m->set_degree(3);
}

void Triangulation::build_first_triangle() {
    assert(vertices_.empty());
    assert(halfedges_.empty());

    // create vertices and edges of first triangle
    Vertex *vertex_a = new_vertex();
    Vertex *vertex_b = new_vertex();
    Vertex *vertex_c = new_vertex();
    Halfedge *halfedge_ab = new_edge();
    Halfedge *halfedge_bc = new_edge();
    Halfedge *halfedge_ca = new_edge();
    Halfedge *halfedge_ba = new_edge();
    Halfedge *halfedge_cb = new_edge();
    Halfedge *halfedge_ac = new_edge();

    // build first triangle
    make_triangle(halfedge_ab, halfedge_bc, halfedge_ca, vertex_c, vertex_a, vertex_b);
    make_triangle(halfedge_ac, halfedge_cb, halfedge_ba, vertex_b, vertex_a, vertex_c);
    make_twins(halfedge_ab, halfedge_ba);
    make_twins(halfedge_bc, halfedge_cb);
    make_twins(halfedge_ca, halfedge_ac);

    // set degree of first three vertices
    vertex_a->set_degree(2);
    vertex_b->set_degree(2);
    vertex_c->set_degree(2);
}

void Triangulation::build_canonical(int n) {
    assert(n >= 4);

    // create first triangle
    build_first_triangle();
    Halfedge *halfedge = this->halfedge(0);

    // apply e3-expansion n-3 times
    for (int i = 3; i < n; ++i) {
        edpand(halfedge);
    }

#ifndef NDEBUG
    check(*this);
#endif
}

void Triangulation::build_dominant_twin_star(int n) {
    assert(n >= 4);

    // create first triangle
    build_first_triangle();
    Halfedge *halfedge_a = halfedge(0);
    Halfedge *halfedge_b = halfedge_a->next();

    // create all other triangles by applying e3-expansions
    for (int i = 3; i < n; ++i) {
        edpand(halfedge_a);
        std::swap(halfedge_a, halfedge_b);
    }

#ifndef NDEBUG
    check(*this);
#endif
}

void Triangulation::build_dominant_zig_zag(int n) {
    assert(n >= 4);

    // create first triangle
    build_first_triangle();
    Halfedge *halfedge = this->halfedge(0);

    // create all other triangles by applying e3-expansions
    for (int i = 3; i < n; ++i) {
        edpand(halfedge);
        if (i % 2 == 0) {
            halfedge = halfedge->prev()->twin();
        } else {
            halfedge = halfedge->next()->twin();
        }
    }

#ifndef NDEBUG
    check(*this);
#endif
}

void Triangulation::build_dominant_binary_tree(int n) {
    assert(n >= 4);

    // create first triangle
    build_first_triangle();
    Halfedge *halfedge = this->halfedge(0);

    edpand(halfedge);

    std::queue<Halfedge *> queue;
    queue.push(halfedge->twin());
    queue.push(halfedge->prev()->twin());
    queue.push(halfedge->next()->twin());

    for (int i = 4; i < n; ++i) {
        halfedge = queue.front();
        queue.pop();

        edpand(halfedge);

        queue.push(halfedge->prev()->twin());
        queue.push(halfedge->next()->twin());
    }

#ifndef NDEBUG
    check(*this);
#endif
}

void Triangulation::build_outerplanar(int n) {
    assert(n >= 4);

    // create first triangle
    build_first_triangle();
    Halfedge *halfedge = this->halfedge(0);
    make_fixed(halfedge);
    make_fixed(halfedge->prev());
    make_fixed(halfedge->next());

    for (int i = 3; i < n; ++i) {
        split(halfedge);
    }

#ifndef NDEBUG
    check(*this);
#endif
}

void Triangulation::build_from_code(const Code &code) {
    assert(vertices_.empty());
    assert(halfedges_.empty());

    int n = (int) code.symbol(0);

    for (int i = 0; i < n; ++i) { new_vertex(); }

    int index = 0;
    int count = 1;

    for (int i = 0; i < n; ++i) {
        Vertex *vertex_a = vertex(i);
        vertex_a->set_label(i + 1);
        vertex_a->set_degree(0);

        // connect vertex with its incident edges
        Halfedge *first = nullptr;
        Halfedge *last = nullptr;
        while (code.symbol(++index)) {
            int j = (int) (code.symbol(index) - 1);
            if (j >= n) { j = count++; }
            Vertex *vertex_b = vertex(j);
            vertex_a->increase_degree();

            Halfedge *current;
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

#ifdef OUTERPLANAR
    // make edges that are adjacent to a non-triangle fixed
    for (Halfedge *halfedge : halfedges_) {
        if (halfedge->next()->next()->next() != halfedge) {
            make_fixed(halfedge);
        }
    }
#endif

#ifndef NDEBUG
    check(*this);
#endif
}

void Triangulation::copy(const Triangulation &triangulation) {
    assert(vertices_.size() == 0);
    assert(halfedges_.size() == 0);

    int n = triangulation.order();
    int m = triangulation.size();
    std::map<Vertex *, Vertex *> vertex_map;
    std::map<Halfedge *, Halfedge *> edge_map;

    // create vertices
    for (int i = 0; i < n; ++i) {
        Vertex *vertex = triangulation.vertex(i);
        Vertex *copy = new_vertex();
        vertex_map[vertex] = copy;
    }

    // create edges
    for (int i = 0; i < m; ++i) {
        Halfedge *halfedge = triangulation.halfedge(i);
        Halfedge *copy = new_edge();
        copy->set_id(halfedge->id());
        edge_map[halfedge] = copy;
    }

    // set members of vertices
    for (int i = 0; i < n; ++i) {
        Vertex *vertex = triangulation.vertex(i);
        Vertex *copy = vertex_map[vertex];
        copy->set_label(vertex->label());
        copy->set_halfedge(edge_map[vertex->halfedge()]);
        copy->set_degree(vertex->degree());
    }

    // set members of edges
    for (int i = 0; i < m; ++i) {
        Halfedge *halfedge = triangulation.halfedge(i);
        Halfedge *copy = edge_map[halfedge];
        copy->set_target(vertex_map[halfedge->target()]);
        copy->set_twin(edge_map[halfedge->twin()]);
        copy->set_prev(edge_map[halfedge->prev()]);
        copy->set_next(edge_map[halfedge->next()]);
    }

#ifndef NDEBUG
    check(*this);
#endif
}

void Triangulation::clear() {
    int n = (int) vertices_.size();;
    int m = (int) halfedges_.size();
    for (int i = 0; i < n; ++i) { delete vertices_[i]; }
    for (int j = 0; j < m; ++j) { delete halfedges_[j]; }
    halfedges_.clear();
    vertices_.clear();
}

int Triangulation::order() const {
    return (int) vertices_.size();
}

int Triangulation::size() const {
    return (int) halfedges_.size();
}

int Triangulation::min_degree() const {
    int minimum = order();
    for (Vertex *vertex : vertices_) {
        minimum = std::min(minimum, vertex->degree());
    }
    return minimum;
}

int Triangulation::max_degree() const {
    int maximum = 0;
    for (Vertex *vertex: vertices_) {
        maximum = std::max(maximum, vertex->degree());
    }
    return maximum;
}

bool Triangulation::has_dominant_vertex() const {
    return max_degree() == order() - 1;
}

Vertex *Triangulation::vertex(int i) const {
    return vertices_[i];
}

Halfedge *Triangulation::halfedge(int i) const {
    return halfedges_[i];
}

Halfedge *Triangulation::halfedge(Vertex *vertex_a, Vertex *vertex_b) const {
    Halfedge *first = vertex_a->halfedge();
    Halfedge *current = first;
    do {
        if (current->target() == vertex_b) { return current; }
        current = current->twin()->next();
    } while (current != first);
    return nullptr;
}

bool Triangulation::is_representative(Halfedge *halfedge) const {
    return halfedge->id() < halfedge->twin()->id();
}

bool Triangulation::is_flippable(Halfedge *halfedge) const {
    // get endpoints
    Vertex *vertex_a = halfedge->next()->target();
    Vertex *vertex_b = halfedge->twin()->next()->target();

    // check whether vertex_a and vertex_b are adjacent
    Halfedge *first = vertex_a->halfedge();
    Halfedge *current = first;
    do {
        if (current->target() == vertex_b) { return false; }
        current = current->twin()->next();
    } while (current != first);
    return true;
}

void Triangulation::flip(Halfedge *halfedge) {
    assert(is_flippable(halfedge));

    Halfedge *twin = halfedge->twin();
    Vertex *vertex_t = halfedge->target();
    Vertex *vertex_s = twin->target();
    Vertex *vertex_a = halfedge->next()->target();
    Vertex *vertex_b = twin->next()->target();
    Halfedge *edge_ta = halfedge->next();
    Halfedge *edge_as = halfedge->prev();
    Halfedge *edge_sb = twin->next();
    Halfedge *edge_bt = twin->prev();

    // perform flip
    make_triangle(halfedge, edge_bt, edge_ta);
    make_triangle(twin, edge_as, edge_sb);
    halfedge->set_target(vertex_b);
    twin->set_target(vertex_a);
    if (vertex_t->halfedge() == twin) { vertex_t->set_halfedge(edge_ta); }
    if (vertex_s->halfedge() == halfedge) { vertex_s->set_halfedge(edge_sb); }

    // update degrees
    vertex_t->decrease_degree();
    vertex_s->decrease_degree();
    vertex_a->increase_degree();
    vertex_b->increase_degree();

#ifndef NDEBUG
    check(*this);
#endif
}

void Triangulation::write_to_stream(std::ostream &output_stream) const {
    int n = order();
    int m = size();
    std::map<Vertex *, int> map;

    output_stream << "graph {" << std::endl;

    for (int i = 0; i < n; ++i) {
        Vertex *vertex = this->vertex(i);
        map[vertex] = i;
        output_stream << "  v" << i;
        output_stream << " [label=" << (char) ('a' + std::max(vertex->label() - 1, 0)) << "];";
        output_stream << std::endl;
    }

    for (int j = 0; j < m; ++j) {
        Halfedge *halfedge = this->halfedge(j);
        Halfedge *twin = halfedge->twin();
        if (is_representative(halfedge) || !is_representative(halfedge->twin())) {
            Vertex *source = twin->target();
            Vertex *target = halfedge->target();
            std::string color = is_flippable(halfedge) ? "blue" : "red";
            output_stream << "  v" << map[source];
            output_stream << " -- v" << map[target];
            output_stream << " [color=" << color << "];";
            output_stream << std::endl;
        }
    }

    output_stream << "}" << std::endl;
}

#ifndef OUTERPLANAR
void Triangulation::check(Triangulation &triangulation) {
    int n = triangulation.order();
    int m = triangulation.size();

    assert(m == 2 * (3 * n - 6));

    for (int i = 0; i < n; ++i) {
        // check vertex pointers
        Vertex *vertex = triangulation.vertex(i);
        assert(vertex == vertex->halfedge()->twin()->target());

        // check vertex degree
        int degree = 0;
        Halfedge *first = vertex->halfedge();
        Halfedge *current = first;
        do {
            degree++;
            current = current->twin()->next();
        } while (current != first);
        assert(vertex->degree() == degree);
    }

    for (int i = 0; i < m; ++i) {
        // check halfedge pointers
        Halfedge *halfedge = triangulation.halfedge(i);
        assert(halfedge == halfedge->twin()->twin());
        assert(halfedge->target() == halfedge->next()->twin()->target());
        assert(halfedge == halfedge->prev()->next());
        assert(halfedge == halfedge->next()->prev());
        assert(halfedge == halfedge->next()->next()->next());
        assert(halfedge == halfedge->prev()->prev()->prev());
        assert(halfedge->next() == halfedge->prev()->prev());
        assert(halfedge->prev() == halfedge->next()->next());
    }

    // handshaking lemma
    int sum = 0;
    for (int i = 0; i < n; ++i) {
        Vertex *vertex = triangulation.vertex(i);
        int degree = vertex->degree();
        assert(degree >= 3);
        sum += degree;
    }
    assert(sum == m);
}
#else

void Triangulation::check(Triangulation &triangulation) {
    int n = triangulation.order();
    int m = triangulation.size();


    for (int i = 0; i < n; ++i) {
        // check vertex pointers
        Vertex *vertex = triangulation.vertex(i);
        assert(vertex == vertex->halfedge()->twin()->target());

        // check vertex degree
        int degree = 0;
        Halfedge *first = vertex->halfedge();
        Halfedge *current = first;
        do {
            degree++;
            current = current->twin()->next();
        } while (current != first);
        assert(vertex->degree() == degree);
    }

    bool outer_cycle = false;
    for (int i = 0; i < m; ++i) {
        // check halfedge pointers
        Halfedge *halfedge = triangulation.halfedge(i);
        assert(halfedge->target() != nullptr); // TODO: remove
        assert(halfedge == halfedge->twin()->twin());
        assert(halfedge->target() == halfedge->next()->twin()->target());
        assert(halfedge == halfedge->prev()->next());
        assert(halfedge == halfedge->next()->prev());
        if (outer_cycle || halfedge == halfedge->next()->next()->next()) {
            assert(halfedge == halfedge->next()->next()->next());
            assert(halfedge == halfedge->prev()->prev()->prev());
            assert(halfedge->next() == halfedge->prev()->prev());
            assert(halfedge->prev() == halfedge->next()->next());
        } else {
            Halfedge *forward = halfedge;
            Halfedge *backward = halfedge;
            for (int i = 0; i < n; ++i) {
                forward = forward->next();
                backward = backward->prev();
            }
            assert(halfedge == forward);
            assert(halfedge == backward);
        }
    }

    // handshaking lemma
    int sum = 0;
    for (int i = 0; i < n; ++i) {
        Vertex *vertex = triangulation.vertex(i);
        int degree = vertex->degree();
        sum += degree;
    }
    assert(sum == m);
}

#endif

/* ---------------------------------------------------------------------- *
 * impementation of the code class
 * ---------------------------------------------------------------------- */

Code::Code(const Triangulation &triangulation) {
    compute_code(triangulation);
}

Code::Code(const Triangulation &triangulation, Halfedge *halfedge) {
    compute_code(triangulation, halfedge);
}

Code::Code(const Code &code) {
    length_ = code.length_;
    code_ = new unsigned char[length_];
    for (int i = 0; i < length_; ++i) { set_symbol(i, code.symbol(i)); }
}

Code::Code(const std::vector<unsigned char> &code) {
    length_ = (int) code.size();
    code_ = new unsigned char[length_];
    for (int i = 0; i < length_; ++i) { set_symbol(i, code[i]); }
}

Code::~Code() {
    delete[] code_;
}

void Code::initialize(const Triangulation &triangulation) {
    int n = triangulation.order();
    int m = triangulation.size();
    length_ = n + m + 1;
    code_ = new unsigned char[length_];
    code_[0] = n;
    for (int i = 1; i < length_; ++i) { code_[i] = 2 * n; }
}

void Code::update(const Triangulation &triangulation, Halfedge *halfedge, bool clockwise) {
    int n = triangulation.order();
    for (int i = 0; i < n; ++i) {
        Vertex *vertex = triangulation.vertex(i);
        vertex->set_label(0);
    }

    int index = 1;
    int label = 1;
    bool smaller = false;
    std::queue<Halfedge *> queue;

    // breadth first search
    halfedge->target()->set_label(label++);
    queue.push(halfedge);
    while (!queue.empty()) {
        Halfedge *first = queue.front()->twin();
        Halfedge *current = first;
        queue.pop();

        do {
            Vertex *vertex = current->target();
            int symbol = vertex->label();

            if (symbol == 0) {
                vertex->set_label(label++);
                queue.push(current);
                symbol = n + vertex->degree();
            }

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

void Code::compute_code(const Triangulation &triangulation) {
    initialize(triangulation);
    int m = triangulation.size();

    // compute minimal degree
    int n = triangulation.order();
    int min_degree = n;
    for (int i = 0; i < n; ++i) {
        Vertex *vertex = triangulation.vertex(i);
        min_degree = std::min(min_degree, vertex->degree());
    }

    for (int i = 0; i < m; ++i) {
        Halfedge *halfedge = triangulation.halfedge(i);
        // only copute codes if target vertex has minimal degree
        if (halfedge->target()->degree() > min_degree) { continue; }
        update(triangulation, halfedge, true);
        update(triangulation, halfedge, false);
    }
}

void Code::compute_code(const Triangulation &triangulation, Halfedge *halfedge) {
    initialize(triangulation);
    Halfedge *twin = halfedge->twin();
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

bool Code::operator==(const Code &other) const {
    if (length_ != other.length_) { return false; }
    for (int i = 0; i < length_; ++i) {
        if (code_[i] != other.code_[i]) { return false; }
    }
    return true;
}

bool Code::operator!=(const Code &other) const {
    return !(*this == other);
}

bool Code::operator<(const Code &other) const {
    if (length_ < other.length_) { return true; }
    if (length_ > other.length_) { return false; }
    for (int i = 0; i < length_; ++i) {
        if (code_[i] < other.code_[i]) { return true; }
        if (code_[i] > other.code_[i]) { return false; }
    }
    return false;
}

bool Code::operator<=(const Code &other) const {
    if (length_ < other.length_) { return true; }
    if (length_ > other.length_) { return false; }
    for (int i = 0; i < length_; ++i) {
        if (code_[i] < other.code_[i]) { return true; }
        if (code_[i] > other.code_[i]) { return false; }
    }
    return true;
}

bool Code::operator>(const Code &other) const {
    return other < *this;
}

bool Code::operator>=(const Code &other) const {
    return other <= *this;
}

void Code::write_to_stream(std::ostream &output_stream) const {
    int n = (int) symbol(0);
    int index = 0;
    output_stream << n;
    int count = 1;
    for (int i = 0; i < n; ++i) {
        output_stream << ' ';
        while (symbol(++index)) {
            int symbol = this->symbol(index);
            if (symbol > n) { symbol = ++count; }
            output_stream << (char) ('a' + symbol - 1);
        }
    }
    output_stream << std::endl;
}

/* ---------------------------------------------------------------------- *
 * end of file
 * ---------------------------------------------------------------------- */