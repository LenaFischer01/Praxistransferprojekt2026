#pragma once

#include <GLFW/glfw3.h>
#include "GLFWContext.h"


class GLFWContextFactory {

    private:
        class Context {
            private:
                GLFWContext glfwContext_;
                GLFWwindow* window_;

            public:
                Context();
                ~Context();

                Context(const Context&) = delete;
                Context& operator=(const Context&) = delete;

                GLFWwindow* getWindow() const;
        };

    public: 
        static Context createContext();
};