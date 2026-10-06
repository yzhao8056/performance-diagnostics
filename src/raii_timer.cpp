// raii_timer.cpp
#include <chrono>
#include <print>

class RaiiTimer {
public:
    RaiiTimer();
    ~RaiiTimer();
    void stop();

    // rule of 0
    RaiiTimer(const RaiiTimer& timer) = delete;
    RaiiTimer& operator=(const RaiiTimer& timer) = delete;
    RaiiTimer(RaiiTimer&& timer) = delete;
    RaiiTimer& operator=(RaiiTimer&& timer) = delete;

private:
    std::chrono::time_point<std::chrono::steady_clock> start_point_;
};

RaiiTimer::RaiiTimer()
    : start_point_(std::chrono::steady_clock::now()) {}

RaiiTimer::~RaiiTimer() {
    stop();
}

void RaiiTimer::stop() {
    std::chrono::time_point<std::chrono::steady_clock> endpoint = std::chrono::steady_clock::now();
    double start = static_cast<double>(std::chrono::time_point_cast<std::chrono::microseconds>(start_point_).time_since_epoch().count());
    double end = static_cast<double>(std::chrono::time_point_cast<std::chrono::microseconds>(endpoint).time_since_epoch().count());
    double duration = end - start;
    // double ms = duration * 0.001;
    std::println("RAII Timer reports duration {} us", duration);
}

void run_heavy_task() {
    int result{};
    for (int i{}; i < 1'000'000; ++i) {
        ++result;
    }
    asm volatile("": : "r"(result));
}

int main() {
    std::println("Starting RAII task...");
    {
        RaiiTimer timer{};
        run_heavy_task();
    }
    return 0;
}
