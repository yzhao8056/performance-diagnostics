// Unique_ptr.hpp
#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>

namespace Custom {

template <typename T>
class UniquePtr {
public:
    explicit constexpr UniquePtr(T* ptr = nullptr) noexcept;
    UniquePtr(const UniquePtr& other) = delete;
    UniquePtr& operator=(const UniquePtr& other) = delete;
    UniquePtr(UniquePtr&& other) noexcept;
    UniquePtr& operator=(UniquePtr&& other) noexcept;
    ~UniquePtr();

    // access
    T* get() const noexcept;
    T& operator*() const;
    T* operator->() const noexcept;
    explicit operator bool() const noexcept;

    // ownership change
    T* release() noexcept;
    void reset(T* ptr = nullptr) noexcept;
    void swap(UniquePtr& other) noexcept;

private:
    T* ptr_;

};

}


#include "Unique_ptr.tpp"
