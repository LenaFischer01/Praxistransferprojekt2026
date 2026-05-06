#pragma once
#include <GLFW/glfw3.h>
#include "UiState.h"

class UI {
public:
    UI(GLFWwindow* window);
    ~UI();

    void startFrame();
    void defineStyleAndUi(UiState::Parameters& params);
    void render();

private:
    GLFWwindow* window_ = nullptr;
    void InitImGui(GLFWwindow* window);
};

