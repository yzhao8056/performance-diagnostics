// Vector.tpp

template <typename T>
Vector<T>::Vector(std::size_t count) {
    if (count == 0) {
        return;
    }

    begin_ = alloc_.allocate(count);
    end_ = begin_;
    capacity_end_ = begin_ + count;

    try {
        for (std::size_t i{}; i < count; ++i) {
            std::construct_at(end_);
            ++end_;
        }
    } catch (...) {
        for (T* p = begin_; p != end_; ++p) {
            std::destroy_at(p);
        }

        alloc_.deallocate(begin_, count);
        throw;
    }
}

template <typename T>
Vector<T>::Vector(std::size_t count, const T& value) {
    if (count == 0) {
        return;
    }

    begin_ = alloc_.allocate(count);
    end_ = begin_;
    capacity_end_ = begin_ + count;

    try {
        for (std::size_t i{}; i < count; ++i) {
            std::construct_at(end_, value);
            ++end_;
        }
    } catch (...) {
        for (T* p = begin_; p != end_; ++p) {
            std::destroy_at(p);
        }

        alloc_.deallocate(begin_, count);
        throw;
    }
}

template <typename T>
Vector<T>::Vector(const Vector& other) {
    std::size_t new_size = other.size();

    if (new_size == 0) {
        return;
    }

    begin_ = alloc_.allocate(new_size);
    end_ = begin_;
    capacity_end_ = begin_ + new_size;

    try {
        for (T* p = other.begin_; p != other.end_; ++p) {
            std::construct_at(end_, *p);
            ++end_;
        }
    } catch (...) {
        for (T* p = begin_; p != end_; ++p) {
            std::destroy_at(p);
        }

        alloc_.deallocate(begin_, new_size);
        throw;
    }
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& other) {
    if (this == &other) {
        return *this;
    }

    const std::size_t new_size = other.size();

    T* new_begin = nullptr;
    T* new_end = nullptr;

    // construct new elements somewhere
    if (new_size != 0) {
        new_begin = alloc_.allocate(new_size);
        new_end = new_begin;

        try { // begin construction
            for (T* p = other.begin_; p != other.end_; ++p) {
                std::construct_at(new_end, *p);
                ++new_end;
            }
        } catch (...) { // destroy everything we've constructed
            for (T* p = new_begin; p != new_end; ++p) {
                std::destroy_at(p);
            }

            alloc_.deallocate(new_begin, new_size);
            throw;
        }
    }

    // destroy old elements
    for (T* p = begin_; p != end_; ++p) {
        std::destroy_at(p);
    }

    // free old region
    if (begin_ != nullptr) {
        alloc_.deallocate(begin_, capacity_end_ - begin_);
    }

    // install new elements
    begin_ = new_begin;
    end_ = new_end;

    if (begin_ != nullptr) {
        capacity_end_ = end_;
    } else {
        capacity_end_ = nullptr;
    }

    return *this;
}

template <typename T>
Vector<T>::Vector(Vector&& other) noexcept
    : begin_(other.begin_),
      end_(other.end_),
      capacity_end_(other.capacity_end_) {

    other.begin_ = nullptr;
    other.end_ = nullptr;
    other.capacity_end_ = nullptr;
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    // Destroy current elements
    for (T* p = begin_; p != end_; ++p) {
        std::destroy_at(p);
    }

    // Release current allocation
    if (begin_ != nullptr) {
        alloc_.deallocate(begin_, capacity_end_ - begin_);
    }

    // Steal other's storage
    begin_ = other.begin_;
    end_ = other.end_;
    capacity_end_ = other.capacity_end_;

    // Leave other empty
    other.begin_ = nullptr;
    other.end_ = nullptr;
    other.capacity_end_ = nullptr;

    return *this;
}

template <typename T>
Vector<T>::~Vector() {
    for (T* p = begin_; p != end_; ++p) {
        std::destroy_at(p);
    }

    if (begin_ != nullptr) {
        alloc_.deallocate(begin_, capacity_end_ - begin_);
    }
}

template <typename T>
T& Vector<T>::operator[](std::size_t index) {
    return begin_[index];
}

template <typename T>
const T& Vector<T>::operator[](std::size_t index) const {
    return begin_[index];
}

template <typename T>
std::size_t Vector<T>::size() const noexcept {
    if (begin_ == nullptr) {
        return 0;
    }
    return static_cast<std::size_t>(end_ - begin_);
}

template <typename T>
std::size_t Vector<T>::capacity() const noexcept {
    if (begin_ == nullptr) {
        return 0;
    }
    return static_cast<std::size_t>(capacity_end_ - begin_);
}

template <typename T>
void Vector<T>::reserve(std::size_t new_capacity) {
    std::size_t current_capacity = 0;

    if (capacity_end_ != nullptr) {
        current_capacity = static_cast<std::size_t>(capacity_end_ - begin_);
    }

    if (new_capacity <= current_capacity) {
        return;
    }

    T* new_begin = alloc_.allocate(new_capacity);
    T* new_end = new_begin;

    try {
        // move or copy over objects
        for (T* p = begin_; p != end_; ++p) {
            std::construct_at(new_end, std::move_if_noexcept(*p));
            ++new_end;
        }
    } catch (...) {
        // destroy if throws
        for (T* p = new_begin; p != new_end; ++p) {
            std::destroy_at(p);
        }
        alloc_.deallocate(new_begin, new_capacity);
        throw;
    }

    // Destroy old elements
    for (T* p = begin_; p != end_; ++p) {
        std::destroy_at(p);
    }

    // Deallocate old elements
    if (begin_ != nullptr) {
        alloc_.deallocate(begin_, capacity_end_ - begin_);
    }

    begin_ = new_begin;
    end_ = new_end;
    capacity_end_ = new_begin + new_capacity;
}

template <typename T>
void Vector<T>::resize(std::size_t new_size) {
    if (new_size < size()) {
        // Destroy old elements
        for (T* p = begin_ + new_size; p != end_; ++p) {
            std::destroy_at(p);
        }
        end_ = begin_ + new_size;

    } else if (new_size > size()) {
        // Allocate capacity if needed
        if (new_size > capacity()) {
            std::size_t new_capacity = capacity() == 0 ? 1 : capacity();

            while (new_size > new_capacity) {
                new_capacity *= 2;
            }

            reserve(new_capacity);
        }

        // Construct new elements
        T* old_end = end_;

        try {
            while (end_ != begin_ + new_size) {
                std::construct_at(end_);
                ++end_;
            }
        } catch (...) {
            while (end_ != old_end) {
                --end_;
                std::destroy_at(end_);
            }

            throw;
        }
    }
}

template <typename T>
void Vector<T>::shrink_to_fit() {
    std::size_t old_size = size();
    std::size_t old_capacity = capacity();

    if (old_size == old_capacity) {
        return;
    }

    if (old_size == 0) {
        if (begin_ != nullptr) {
            alloc_.deallocate(begin_, old_capacity);
        }

        begin_ = nullptr;
        end_ = nullptr;
        capacity_end_ = nullptr;
        return;
    }

    T* new_begin = alloc_.allocate(old_size);
    T* new_end = new_begin;

    try {
        for (T* p = begin_; p != end_; ++p) {
            std::construct_at(new_end, std::move_if_noexcept(*p));
            ++new_end;
        }
    } catch (...) {
        for (T* p = new_begin; p != new_end; ++p) {
            std::destroy_at(p);
        }

        alloc_.deallocate(new_begin, old_size);
        throw;
    }

    for (T* p = begin_; p != end_; ++p) {
        std::destroy_at(p);
    }

    alloc_.deallocate(begin_, old_capacity);

    begin_ = new_begin;
    end_ = new_end;
    capacity_end_ = new_end;
}

template <typename T>
bool Vector<T>::empty() const noexcept {
    return begin_ == end_;
}

template <typename T>
void Vector<T>::push_back(const T& value) {
    if (end_ == capacity_end_) {
        T temp(value);

        std::size_t new_capacity = capacity() == 0 ? 1 : capacity();
        reserve(new_capacity * 2);

        std::construct_at(end_, std::move(temp));
    } else {
        std::construct_at(end_, value);
    }

    ++end_;
}

template <typename T>
void Vector<T>::push_back(T&& value) {
    if (end_ == capacity_end_) {
        T temp(std::move(value));

        std::size_t new_capacity = capacity() == 0 ? 1 : capacity();
        reserve(new_capacity * 2);

        std::construct_at(end_, std::move(temp));
    } else {
        std::construct_at(end_, std::move(value));
    }

    ++end_;
}

template <typename T>
void Vector<T>::pop_back() {
    if (empty()) {
        return;
    }

    --end_;
    std::destroy_at(end_);
}
