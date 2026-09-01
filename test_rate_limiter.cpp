// test_rate_limiter.cpp
#include "rate_limiter.hpp"

#include <cassert>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

void test_1_global_limiter() {
    RateLimiter rl(2, 50ms);
    RateLimiter::Clock::time_point start = RateLimiter::Clock::now();

    const auto first = rl.call("global");
    assert(first.accepted);
    assert(first.remaining == 1);

    const auto second = rl.call("global");
    assert(second.accepted);
    assert(second.remaining == 0);

    const auto third = rl.call("global");
    assert(!third.accepted);
    assert(third.remaining == 0);

    std::this_thread::sleep_until(start + 60ms);

    assert(rl.canCall());
    assert(rl.getCount() == 0);

    const auto fourth = rl.call("global");
    assert(fourth.accepted);
    assert(fourth.remaining == 1);

    std::cout << "Test 1 passed.\n";
}

int main() {
    test_1_global_limiter();
    return 0;
}