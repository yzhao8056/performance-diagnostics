// Vector.hpp
#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>
#include <memory>
#include <cassert>
#include <print>
#include <utility>

namespace Custom {

template <typename T>
class Vector {
public:
    Vector() = default; // default constructor
    explicit Vector(std::size_t count); // constructor
    Vector(std::size_t count, const T& value); // constructor

    Vector(const Vector& other); // copy constructor
    Vector& operator=(const Vector& other); // copy assignment operator

    Vector(Vector&& other) noexcept; // move constructor
    Vector& operator=(Vector&& other) noexcept; // move assignment operator

    ~Vector(); // destructor

    T& operator[](std::size_t index);
    const T& operator[](std::size_t index) const;

    std::size_t size() const noexcept;
    std::size_t capacity() const noexcept;
    void reserve(std::size_t new_capacity);
    void resize(std::size_t new_size);
    void shrink_to_fit();
    bool empty() const noexcept;

    void push_back(const T&value);
    void push_back(T&& value);
    void pop_back();

    template <typename... Args>
    void emplace_back(Args&&... args);

private:
    std::allocator<T> alloc_;
    T* begin_ = nullptr;
    T* end_ = nullptr;
    T* capacity_end_ = nullptr;
};

}


#include "Vector.tpp"
