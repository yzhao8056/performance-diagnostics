// rate_limiter.cpp
#include "rate_limiter.hpp"

#include <stdexcept>

RateLimiter::RateLimiter(std::size_t capacity, Duration period)
    : ring_(capacity), period_(period) {
    if (period < Duration::zero()) {
        throw std::invalid_argument("period must be positive.");
    }
}

void RateLimiter::expireOldCalls(Clock::time_point now) {
    while (count_ > 0 && now - ring_[head_].timestamp >= period_) {
        head_ = (head_ + 1) % ring_.size();
        --count_;
    }
}

bool RateLimiter::canCallAt(Clock::time_point now) {
    if (ring_.empty()) {
        return false;
    }

    expireOldCalls(now);
    return count_ < ring_.size();
}

bool RateLimiter::canCall() {
    return canCallAt(Clock::now());
}

RateLimiterResp RateLimiter::call(const std::string& token) {
    Clock::time_point now = Clock::now();

    if (!canCall()) {
        return { false, 0 };
    }

    const std::size_t tail = (head_ + count_) % ring_.size();
    ring_[tail] = { token, now };
    ++count_;

    return { true, ring_.size() - count_ };
}

std::size_t RateLimiter::getCount() {
    return count_;
}