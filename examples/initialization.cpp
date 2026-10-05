// initialization.cpp
#include <print>
#include <iostream>

struct InnerStruct {
    char c;
    int x;
};

struct DefaultStruct {
    int x;
    int y;
    InnerStruct is;
};

struct ConstructorStruct {
    int x;
    int y;

    void operator()(int a, int b) {
        x = a;
    }
};

int main() {
    DefaultStruct d;
    std::println("UB: {}", d.x);
    std::cout << "d.x_addr: " << &d.x << std::endl;
    DefaultStruct di{.x=0};
    std::println("Designated Initialization: {}", di.x);
    DefaultStruct ds{};
    std::println("Default Initialization: x = {}, is.c = {}, is.x = {}", ds.x, ds.is.c, ds.is.x);
    return 0;
}
