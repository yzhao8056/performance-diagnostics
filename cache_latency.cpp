// g++ -O3 -std=c++20 -march=native cache_latency.cpp -o cache_latency
// ./cache_latency > latency_results.csv

#include <iostream>
#include <chrono>
#include <algorithm>
#include <cstdint> // uint32_t
#include <cstddef> // size_t
#include <vector>
#include <iomanip>
#include <numeric>
#include <random>

using Clock = std::chrono::steady_clock;

int main() {
    constexpr std::size_t min_size = 1ULL << 10; // 1 KiB
    constexpr std::size_t max_size = 1ULL << 29; // 512 MiB
    constexpr std::size_t target_accesses = 100'000'000;

    std::mt19937_64 rng(12345);

    std::cout << "size_kib,accesses,latency_ns\n";

    for (std::size_t bytes = min_size; bytes <= max_size; bytes *= 2) {
        const std::size_t count = bytes / sizeof(std::uint32_t);

        std::vector<std::uint32_t> order(count);
        std::iota(order.begin(), order.end(), 0);
        std::shuffle(order.begin(), order.end(), rng);

        std::vector<std::uint32_t> next(count);

        for (std::size_t i = 0; i + 1 < count; i++) {
            next[order[i]] = order[i+1];
        }

        next[order.back()] = order.front();

        std::uint32_t idx = order.front();

        // warm up to avoid one-time faults
        for (std::size_t i = 0; i < count; i++) {
            idx = next[idx];
        }

        const std::size_t accesses = std::max<std::size_t>(target_accesses, count);

        const auto start = Clock::now();

        for (std::size_t i = 0; i < accesses; i++) {
            idx = next[idx];
        }

        const auto end = Clock::now();

        // prevent compiler optimizations
        if (idx == static_cast<uint32_t>(-1)) {
            std::cerr << "Erhmm\n";
        }

        const double seconds = std::chrono::duration<double>(end - start).count();

        const double ns_per_access = seconds * 1e9 / static_cast<double>(accesses);

        std::cout << (bytes / 1024) << ','
                  << accesses << ','
                  << std::fixed << std::setprecision(2)
                  << ns_per_access << '\n';
    }
}