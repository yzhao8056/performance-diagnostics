// test_batch_set.cpp
#include "batch_set.hpp"

#include <cassert>
#include <iostream>

void test_1_basic_set_ops() {
    BatchSet s{1, 2, 3};
    assert(!s.empty());
    assert(s.contains(1));
    assert(s.size() == 3);

    s.insert(4);
    assert(s.contains(4));
    assert(s.size() == 4);
    assert(!s.contains(10));

    std::cout << ">> Test 1 Passed.\n";
}

void test_2_insert_batch() {
    BatchSet s{1, 2, 3};
    std::vector<int> more{4, 5, 6};
    s.insert_batch(more);
    assert(s.size() == 6);
    assert(s.contains(4));

    std::cout << ">> Test 2 Passed.\n";
}

int main() {
    test_1_basic_set_ops();
    test_2_insert_batch();
}