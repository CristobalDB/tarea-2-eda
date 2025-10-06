#pragma once
#include <chrono>
#include <cstdint>

// Pequeñas utilidades de tiempo para medición en tests
struct Timer {
    using clock = std::chrono::high_resolution_clock;
    clock::time_point t0;
    void start() { t0 = clock::now(); }
    double ms() const {
        auto dt = std::chrono::duration<double, std::milli>(clock::now() - t0);
        return dt.count();
    }
};
