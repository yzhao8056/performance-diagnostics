// Unique_ptr.tpp

namespace Custom {

template <typename T, typename ...Args>
UniquePtr<T> make_unique(Args&&... args) {
    return UniquePtr<T>(
        new T(std::forward<Args>(args)...)
    );
}

template <typename T>
constexpr UniquePtr<T>::UniquePtr(T* ptr) noexcept 
    : ptr_(ptr) {}

template <typename T>
UniquePtr<T>::UniquePtr(UniquePtr&& other) noexcept 
    : ptr_(other.release()) {}

template <typename T>
UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr&& other) noexcept {
    if (this != &other) {
        reset(other.release());
    }
    return *this;
}

template <typename T>
UniquePtr<T>::~UniquePtr() {
    delete ptr_;
    ptr_ = nullptr; // don't think we need this line
}

template <typename T>
T* UniquePtr<T>::get() const noexcept {
    return ptr_;
}

template <typename T>
T& UniquePtr<T>::operator*() const {
    return *ptr_;
}

template <typename T>
T* UniquePtr<T>::operator->() const noexcept{
    return ptr_;
}

template <typename T>
UniquePtr<T>::operator bool() const noexcept {
    return ptr_ != nullptr;
}

template <typename T>
T* UniquePtr<T>::release() noexcept {
    T* old = ptr_;
    ptr_ = nullptr;
    return old;
}

template <typename T>
void UniquePtr<T>::reset(T* ptr) noexcept {
    if (ptr == ptr_) {
        return;
    }

    T* old = ptr_;
    ptr_ = ptr;
    delete old;
}

template <typename T>
void UniquePtr<T>::swap(UniquePtr& other) noexcept {
    std::swap(ptr_, other.ptr_);
}

}
