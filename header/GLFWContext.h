#pragma once

#include <GLFW/glfw3.h>
#include <stdexcept>

using namespace std;

class GLFWContext{
    public:
        GLFWContext(){
            if (!glfwInit()) {
                throw runtime_error("GLFW initialization failed");
            }
        };
        ~GLFWContext() { glfwTerminate(); };

        GLFWContext(const GLFWContext&) = delete;
        GLFWContext& operator=(const GLFWContext&) = delete;
};