// batch_set.tpp
#pragma once

#include "batch_set.hpp"

template<class Key, class Hasher, class KeyEqual>
BatchSet<Key,Hasher,KeyEqual>::BatchSet(std::span<const Key> keys) {
    insert_batch(keys);
}

template<class Key, class Hasher, class KeyEqual>
BatchSet<Key,Hasher,KeyEqual>::BatchSet(std::initializer_list<Key> keys)
    : BatchSet(std::span<const Key>{keys.begin(), keys.end()}) {}

template<class Key, class Hasher, class KeyEqual>
bool BatchSet<Key,Hasher,KeyEqual>::insert(const Key& key) {
    return insert_impl(key);
}

template<class Key, class Hasher, class KeyEqual>
bool BatchSet<Key,Hasher,KeyEqual>::insert(Key&& key) {
    return insert_impl(std::move(key));
}

template<class Key, class Hasher, class KeyEqual>
void BatchSet<Key,Hasher,KeyEqual>::insert_batch(std::span<const Key> keys) {
    reserve(size_ + keys.size());

    for (const Key& key : keys) {
        insert(key);
    }
}

template<class Key, class Hasher, class KeyEqual>
bool BatchSet<Key,Hasher,KeyEqual>::contains(const Key& key) const {
    if (buckets_.empty()) {
        return false;
    }

    std::size_t index = bucket_for(key);
    const std::size_t mask = buckets_.size() - 1;

    for (std::size_t probe{}; probe < buckets_.size(); probe++) {
        if (!occupied_[index]) {
            return false;
        }

        if (equal_(buckets_[index], key)) {
            return true;
        }

        index = (index + 1) & mask;
    }

    return false;
}

template<class Key, class Hasher, class KeyEqual>
void BatchSet<Key,Hasher,KeyEqual>::reserve(std::size_t capacity) {
    if (capacity <= max_elements(buckets_.size())) {
        return;
    }

    std::size_t required = (capacity * lf_denominator + lf_numerator - 1) / lf_numerator; // ceil(cap/lf)
    if (required < init_bucket_count) {
        required = init_bucket_count;
    }

    rehash(std::bit_ceil(required));
}

template<class Key, class Hasher, class KeyEqual>
std::size_t BatchSet<Key,Hasher,KeyEqual>::bucket_for(const Key& key) const {
    return hasher_(key) & (buckets_.size() - 1); // buckets_.size() == 1<<x -> bitwise mod
}

template<class Key, class Hasher, class KeyEqual>
std::size_t BatchSet<Key,Hasher,KeyEqual>::max_elements(std::size_t num_buckets) {
    return num_buckets * lf_numerator / lf_denominator;
}

template<class Key, class Hasher, class KeyEqual>
template<class Value>
bool BatchSet<Key,Hasher,KeyEqual>::insert_impl(Value&& key) {
    if (buckets_.empty()) {
        rehash(init_bucket_count);
    }

    std::size_t index = bucket_for(key);
    const std::size_t mask = buckets_.size() - 1;

    for (std::size_t probe{}; probe < buckets_.size(); probe++) {
        if (occupied_[index]) {
            if (equal_(buckets_[index], key)) {
                return false;
            }
        } else {
            if (size_ + 1 > max_elements(buckets_.size())) {
                rehash(buckets_.size() * 2);
                return insert_impl(std::forward<Value>(key));
            }
            buckets_[index] = std::forward<Value>(key);
            occupied_[index] = 1;
            ++size_;
            return true;
        }

        index = (index + 1) & mask;
    }

    rehash(buckets_.size() * 2);
    return insert_impl(std::forward<Value>(key));
}

template<class Key, class Hasher, class KeyEqual>
void BatchSet<Key,Hasher,KeyEqual>::rehash(std::size_t new_bucket_count) {
    if (new_bucket_count < init_bucket_count) {
        new_bucket_count = init_bucket_count;
    }
    new_bucket_count = std::bit_ceil(new_bucket_count);

    std::vector<Key> old_buckets = std::move(buckets_);
    std::vector<std::uint8_t> old_occupied = std::move(occupied_);

    buckets_.clear();
    buckets_.resize(new_bucket_count);
    occupied_.assign(new_bucket_count, 0);
    size_ = 0;

    for (std::size_t i{}; i < old_buckets.size(); i++) {
        if (old_occupied[i]) {
            insert(std::move(old_buckets[i]));
        }
    }
}