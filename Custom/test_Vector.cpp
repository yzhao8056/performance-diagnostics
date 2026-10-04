// test_Vector.cpp
#include "Vector.hpp"
#include <print>

using namespace Custom;

void test_vector_default_constructor() {
    Vector<int> v;

    assert(v.empty());
    assert(v.size() == 0);
}

void test_vector_count_constructor() {
    Vector<int> v(5);

    assert(!v.empty());
    assert(v.size() == 5);
}

void test_vector_indexing() {
    Vector<int> v(5, 42);

    assert(!v.empty());
    assert(v.size() == 5);
    assert(v[0] == 42);

    v[0] = 52;

    assert(v[0] == 52);
    assert(v[1] == 42);
}

void test_vector_copy_constructor() {
    Vector<int> a(5, 42);
    Vector<int> b = a;

    assert(b.size() == 5);
    assert(b[0] == 42);
    assert(b[1] == 42);

    b[0] = 52;

    assert(b[0] == 52);
    assert(a[0] == 42);
}

void test_vector_copy_assignment() {
    Vector<int> a(5, 42);
    Vector<int> b(3, 22);
    b = a;

    assert(b.size() == 5);
    assert(b[0] == 42);

    a[0] = 30;

    assert(b[0] == 42);
}

void test_vector_move_constructor() {
    Vector<int> a(5, 42);
    Vector<int> b = std::move(a);

    assert(a.size() == 0);
    assert(b.size() == 5);
    assert(b[0] == 42);
}

void test_vector_move_assignment() {
    Vector<int> a(5, 42);
    Vector<int> b(3, 22);
    b = std::move(a);

    assert(a.size() == 0);
    assert(b.size() == 5);
    assert(b[0] == 42);
}

int main() {
    test_vector_default_constructor();
    test_vector_count_constructor();
    test_vector_indexing();
    test_vector_copy_constructor();
    test_vector_copy_assignment();
    test_vector_move_constructor();
    test_vector_move_assignment();
    std::println("All test cases passed!");
    return 0;
}
