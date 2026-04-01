#pragma once
#include <GLFW/glfw3.h>

class UI {
public:
    UI(GLFWwindow* window);
    ~UI();

    void startFrame();
    void defineStyleAndUi();
    void render();

    float exposure = 1.0f;
    bool  useToneMapping = true; // TODO: Verändern und echte einbauen

private:
    GLFWwindow* window_ = nullptr;
    void InitImGui(GLFWwindow* window);
};

