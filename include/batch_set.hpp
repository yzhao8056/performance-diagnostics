// batch_set.hpp
#pragma once

#include <span>
#include <functional>
#include <cstddef>
#include <initializer_list>
#include <vector>
#include <utility>
#include <bit>
#include <cstdint>

template<
    class Key,
    class Hasher = std::hash<Key>,
    class KeyEqual = std::equal_to<Key>
>
class BatchSet {
public:
    BatchSet() = default;
    explicit BatchSet(std::span<const Key> keys);
    explicit BatchSet(std::initializer_list<Key> keys);

    bool insert(const Key& key);
    bool insert(Key&& key);
    void insert_batch(std::span<const Key> keys);
    bool contains(const Key& key) const;
    void reserve(std::size_t capacity);

    [[nodiscard]] std::size_t size() const noexcept { return size_; }
    [[nodiscard]] bool empty() const noexcept { return size_ == 0; }

private:
    static constexpr size_t init_bucket_count = 1ULL<<3;
    static constexpr size_t lf_numerator = 7;
    static constexpr size_t lf_denominator = 10;

    Hasher hasher_;
    KeyEqual equal_;
    std::vector<Key> buckets_;
    std::vector<std::uint8_t> occupied_;
    std::size_t size_{};

    std::size_t bucket_for(const Key& key) const;
    [[nodiscard]] static std::size_t max_elements(std::size_t buckets);
    template<class Value> bool insert_impl(Value&& key);
    void rehash(std::size_t new_bucket_count);
};

#include "batch_set.tpp"