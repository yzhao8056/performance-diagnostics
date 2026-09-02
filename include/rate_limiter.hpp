// rate_limiter.hpp
#pragma once

#include <chrono>
#include <cstddef>
#include <string>
#include <vector>

struct RateLimiterResp {
    bool accepted;
    std::size_t remaining;
};

class RateLimiter {
public:
    using Clock = std::chrono::steady_clock;
    using Duration = Clock::duration;

    RateLimiter(std::size_t capacity, Duration period);

    bool canCall();
    RateLimiterResp call(const std::string& token);
    std::size_t getCount();

private:
    struct Entry {
        std::string token;
        Clock::time_point timestamp;
    };
    std::vector<Entry> ring_;
    Duration period_;
    std::size_t head_ = 0;
    std::size_t count_ = 0;

    void expireOldCalls(Clock::time_point now);
    bool canCallAt(Clock::time_point now);
};