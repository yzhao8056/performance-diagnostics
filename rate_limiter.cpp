// rate_limiter.cpp
#include <chrono>
#include <unordered_map>
#include <thread>
#include <string>
#include <cstddef>
#include <vector>

using Clock = std::chrono::steady_clock;
using Duration = Clock::duration;

struct RateLimiterResp {
    bool accepted;
    std::size_t remaining;
};

struct RateLimiterEntry {
    std::string token;
    Clock::time_point timestamp;
};

class RateLimiter {
private:
    std::vector<RateLimiterEntry> ring_;
    Duration period_;
    std::size_t head_ = 0;
    std::size_t count_ = 0;

    void expireOldCalls(Clock::time_point now) {
        while (count_ > 0 && now - ring_[head_].timestamp >= period_) {
            head_ = (head_ + 1) % ring_.size();
            --count_;
        }
    }

    bool canCallAt(Clock::time_point now) {
        if (ring_.empty()) {
            return false;
        }

        expireOldCalls(now);
        return count_ < ring_.size();
    }

public:
    RateLimiter(std::size_t window_size, Duration period)
        : ring_(window_size),period_(period){
            if (period < Duration::zero()) {
                throw std::invalid_argument(
                    "period must be positive"
                );
            }
        }

    bool canCall() {
        return canCallAt(Clock::now());
    }

    RateLimiterResp call(const std::string& token) {
        Clock::time_point now = Clock::now();

        if (!canCall()) {
            return { false, 0 };
        }

        const std::size_t tail = (head_ + count_) % ring_.size();
        ring_[tail] = { token, now };
        ++count_;

        return { true, ring_.size() - count_ };
    }
};

int main() {
    return 0;
}