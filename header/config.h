#ifndef CONFIG_H
#define CONFIG_H

namespace Config {
    constexpr int WINDOW_WIDTH = 800;
    constexpr int WINDOW_HEIGHT = 600;
    constexpr const char* WINDOW_TITLE = "Doppelpendel";

    constexpr double DT = 0.002;

    constexpr float CIRCLE_RADIUS = 0.07f;
    constexpr int CIRCLE_SEGMENTS = 64;
    constexpr int TRACE_POINTS = 1000; // Default 
    constexpr float LINE_WIDTH = 2.5f;

    constexpr const char* VERTEX_SHADER = "shaders/3.3.shader.vs";
    constexpr const char* FRAGMENT_SHADER = "shaders/3.3.shader.fs";
}

#endif