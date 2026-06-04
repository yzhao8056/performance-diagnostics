// logger_rate_limiter.cpp
#include <unordered_map>
#include <chrono>
#include <string>
#include <thread>
#include <iostream>

using Clock = std::chrono::steady_clock;

template<typename Duration = std::chrono::seconds>
class LoggerRateLimiter {
private:
    std::unordered_map<std::string, Clock::time_point> next_allowed_time_;
    Duration restriction_;

public:
    explicit LoggerRateLimiter(int restriction = Duration{10})
        : restriction_(restriction){
    };

    bool shouldPrintMessage(const std::string& message) {
        const auto now = Clock::now();

        if (next_allowed_time_.contains(message) && now < next_allowed_time_[message]) {
            return false;
        }

        next_allowed_time_[message] = now + restriction_;
        return true;
    }
};

int main() {
    LoggerRateLimiter<> rateLimiter(3);
    bool first = rateLimiter.shouldPrintMessage("hi"); // should print true
    bool second = rateLimiter.shouldPrintMessage("hi"); // should print false
    std::this_thread::sleep_for(std::chrono::seconds(3));
    bool third = rateLimiter.shouldPrintMessage("hi"); // should print true

    std::cout << "First result should be true: " << first << '\n'
              << "Second result should be false: " << second << '\n'
              << "Third result should be true: " << third << '\n';
    return 0;
}