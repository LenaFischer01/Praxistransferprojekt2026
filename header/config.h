#ifndef CONFIG_H
#define CONFIG_H

namespace Config {
    constexpr int WINDOW_WIDTH = 800;
    constexpr int WINDOW_HEIGHT = 600;
    constexpr const char* WINDOW_TITLE = "Doppelpendel";

    constexpr double DT = 0.001;
    constexpr double G = 9.81;

    constexpr float CIRCLE_RADIUS = 0.05f;
    constexpr int CIRCLE_SEGMENTS = 64;

    constexpr const char* VERTEX_SHADER = "shaders/3.3.shader.vs";
    constexpr const char* FRAGMENT_SHADER = "shaders/3.3.shader.fs";
}

#endif