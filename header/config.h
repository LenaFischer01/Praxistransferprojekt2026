#pragma once

namespace Config {

    struct Color {
        float r, g, b, a;
    };

    constexpr Color COLOR_TRACE = {0.933333f, 0.686275f, 0.380392f, 1.0f};
    constexpr Color COLOR_LINES = {0.415686f, 0.050980f, 0.513726f, 1.0f};
    constexpr Color COLOR_CIRCLES = {0.933333f, 0.364706f, 0.423529f, 1.0f};
    
    constexpr int WINDOW_WIDTH = 800;
    constexpr int WINDOW_HEIGHT = 600;
    constexpr const char* WINDOW_TITLE = "Doppelpendel";

    constexpr double DT = 0.01;

    constexpr float CIRCLE_RADIUS = 0.07f;
    constexpr int CIRCLE_SEGMENTS = 64;
    constexpr int TRACE_POINTS = 1000; // Default 
    constexpr float LINE_WIDTH = 2.5f;

    constexpr const char* VERTEX_SHADER = "shaders/3.3.shader.vs";
    constexpr const char* FRAGMENT_SHADER = "shaders/3.3.shader.fs";
}
