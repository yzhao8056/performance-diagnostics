// test_Unique_ptr.cpp
#include "Unique_ptr.hpp"

using namespace Custom;

void test_unique_ptr_make() {
    Unique_ptr<int> up = make_unique<int>();
}

int main() {
    return 0;
}
