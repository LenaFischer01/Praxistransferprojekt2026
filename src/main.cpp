#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "shader.h"
#include "config.h"
#include "renderer.h"
#include "ui.h"
#include "fpsCounter.h"
#include "GLFWContext.h"
#include "simulation.h"
#include "UiState.h"

using namespace std;

// ----------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main() {
    try {
        // ------------------------------------------------------------
        // OpenGL Kontext erstellen und initialisieren
        // ------------------------------------------------------------

        GLFWContext glfwContext;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow* window = glfwCreateWindow(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, Config::WINDOW_TITLE, NULL, NULL);
        if (!window) {
            throw std::runtime_error("Failed to create GLFW window");
        }

        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            cout << "Failed to initialize GLAD" << endl;
            return -1;
        }

        // ------------------------------------------------------------
        // Simulation initialisieren
        // ------------------------------------------------------------

        Shader customShader(Config::VERTEX_SHADER, Config::FRAGMENT_SHADER);
        Renderer renderer(customShader);
        UI ui(window);
        FpsCounter fpsCounter;
        Simulation simulation;
        UiState uiState;

        float x1, y1, x2, y2;

        // ------------------------------------------------------------
        // Main Loop
        // ------------------------------------------------------------
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
            processInput(window);

            UiState::Parameters& params = uiState.getParams();

            ui.startFrame();
            ui.defineStyleAndUi(params);

            if (params.resetRequested) {
                uiState.resetParameters();
                renderer.resetRenderer(params.traceLength);
            }
            
            if (params.traceLengthChanged) {
                renderer.resetRenderer(params.traceLength); 
                params.traceLengthChanged = false;
            }

            simulation.setParameters(params.length1, params.length2, params.mass1, params.mass2, params.gravity);

            renderer.setCircleMultiplier(params.mass1 * 0.5f, params.mass2 * 0.5f);

            if (!params.run) {
                simulation.reset(params.angle1, params.angle2);
                renderer.resetRenderer(params.traceLength); 
            }

            if (params.run) {
                simulation.update(Config::DT);
            }
            simulation.getCoordinates(x1, y1, x2, y2);

            renderer.update(x1, y1, x2, y2);

            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            renderer.draw(simulation.getScale(), params.showTrace);

            ui.render();

            glfwSwapBuffers(window);

            if (fpsCounter.update()) {
                string title = string(Config::WINDOW_TITLE) + " - FPS: " + to_string(int(fpsCounter.fps));
                glfwSetWindowTitle(window, title.c_str());
            }

        }

    } catch (const exception& e) {
        cerr << "Fatal error: " << e.what() << endl;
        return -1;
    }

    return 0;
}

// ------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}