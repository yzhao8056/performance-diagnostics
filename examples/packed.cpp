// packed.cpp
#include <print>

struct [[gnu::packed]] PackedStruct {
    char c;
    int i;
};

struct [[gnu::packed, gnu::aligned(64)]] PackedAlignedStruct {
    char c;
    int i;
};

struct [[gnu::aligned(64)]] AlignedStruct {
    char c;
    int i;
};

class [[gnu::packed]] MyClass {
    char c;
    int i;
};

int main() {
    std::println("PackedStruct: sizeof {}, alignof {}", sizeof(PackedStruct), alignof(PackedStruct));
    std::println("PackedAlignedStruct: sizeof {}, alignof {}", sizeof(PackedAlignedStruct), alignof(PackedAlignedStruct));
    std::println("AlignedStruct: sizeof {}, alignof {}", sizeof(AlignedStruct), alignof(AlignedStruct));
    std::println("MyClass: size {}", sizeof(MyClass));
}
