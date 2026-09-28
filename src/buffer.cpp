// buffer.cpp

#include <cstddef>
#include <algorithm>

class Buffer {
public:
    explicit Buffer(size_t n)
    : n_(n), buf_(new int[n]) {}
    
    ~Buffer() {
        delete[] buf_;
    }

    Buffer(const Buffer& other)
    : n_(other.n_), buf_(new int[other.n_]) {
        std::copy(other.buf_, other.buf_+n_, buf_);
    }
    
    Buffer(Buffer&& other)
    : n_(other.n_) {
        other.n_ = 0;
        other.buf_ = nullptr;
    }

private:
    size_t n_;
    int* buf_;
};

int main() {
    return 0;
}