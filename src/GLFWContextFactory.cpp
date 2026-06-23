#include "GLFWContextFactory.h"
#include "config.h"
#include <stdexcept>

GLFWContextFactory::Context::Context()
    : glfwContext_(), window_(nullptr) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_ = glfwCreateWindow(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, Config::WINDOW_TITLE, NULL, NULL);
    if (!window_) {
        throw std::runtime_error("Failed to create GLFW window");
    }
}

GLFWContextFactory::Context::~Context() {
    if (window_) {
        glfwDestroyWindow(window_);
    }
}

GLFWwindow* GLFWContextFactory::Context::getWindow() const {
    return window_;
}

GLFWContextFactory::Context GLFWContextFactory::createContext() {
    return Context();
}