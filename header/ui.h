#pragma once
#include <GLFW/glfw3.h>

class UI {
public:
    UI(GLFWwindow* window);
    ~UI();

    void startFrame();
    void defineStyleAndUi();
    void render();

    struct parameters {
        bool  showTrace = true; 
        float length1 = 1.0f;
        float length2 = 1.0f;
        float mass1 = 1.0f;
        float mass2 = 1.0f;
        float gravity = 9.81f;
        float angle1 = 0.0f;
        float angle2 = 0.0f;

        int traceLength = 1000;
        bool traceLengthChanged = false;

        bool run = false;
    } params;

    parameters& getParams() { return params; }

    void resetParameters() { params = parameters(); }

private:
    GLFWwindow* window_ = nullptr;
    void InitImGui(GLFWwindow* window);
};

