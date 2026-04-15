#ifndef FPSCOUNTER_H
#define FPSCOUNTER_H

#include <chrono>
#include <string>

struct FpsCounter {
    using clock = std::chrono::high_resolution_clock;
    clock::time_point lastTime = clock::now();
    int frames = 0;
    double fps = 0.0;

    // Call once per frame:
    bool update() {
        frames++;
        auto now = clock::now();
        std::chrono::duration<double> elapsed = now - lastTime;

        if (elapsed.count() >= 1.0) {
            fps = frames / elapsed.count();
            frames = 0;
            lastTime = now;
            return true; // fps updated
        }
        return false;
    }
};

#endif