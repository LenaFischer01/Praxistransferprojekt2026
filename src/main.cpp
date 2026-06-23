#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>

#include "shader.h"
#include "config.h"
#include "renderer.h"
#include "ui.h"
#include "fpsCounter.h"
#include "simulation.h"
#include "UiState.h"
#include "GLFWContextFactory.h"

using namespace std;

// ----------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main() {
    try {
        // ------------------------------------------------------------
        // OpenGL (GLFW und GLAD) Kontext erstellen und initialisieren
        // ------------------------------------------------------------

        auto glfwContext = GLFWContextFactory::createContext();
        GLFWwindow* window = glfwContext.getWindow();

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

        // Timing für framerate-unabhängige Simulation
        auto lastFrameTime = std::chrono::high_resolution_clock::now();

        // ------------------------------------------------------------
        // Main Loop
        // ------------------------------------------------------------
        while (!glfwWindowShouldClose(window))
        {
            // Berechne Delta-Zeit in Sekunden
            auto currentTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> deltaTime = currentTime - lastFrameTime;
            lastFrameTime = currentTime;
            
            // Events abrufen, Eingaben verarbeiten
            glfwPollEvents();
            processInput(window);

            UiState::Parameters& params = uiState.getParams();

            // UI starten und Parameter aktualisieren
            ui.startFrame();
            ui.defineStyleAndUi(params);

            // Parameterflags prüfen
            if (params.resetRequested) {
                uiState.resetParameters();
                renderer.resetRenderer(params.traceLength);
            }
            if (params.traceLengthChanged) {
                renderer.resetRenderer(params.traceLength); 
                params.traceLengthChanged = false;
            }

            // Simulation und Renderer aktualisieren
            simulation.setParameters(params.length1, params.length2, params.mass1, params.mass2, params.gravity);
            renderer.setCircleMultiplier(params.mass1 * 0.5f, params.mass2 * 0.5f);

            // Parameterflags prüfen ob die Simulation zurückgesetzt werden soll oder ob sie läuft
            if (!params.run) {
                simulation.reset(params.angle1, params.angle2);
                renderer.resetRenderer(params.traceLength); 
            }
            if (params.run) {
                simulation.update(deltaTime.count(), params.method);
            }

            // Koordinaten der Pendel berechnen
            simulation.getCoordinates(x1, y1, x2, y2);

            // Renderer aktualisieren
            renderer.update(x1, y1, x2, y2);

            // Farben und Buffer löschen, Renderer zeichnen, UI rendern, Fenster tauschen
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            renderer.draw(simulation.getScale(), params.showTrace);
            ui.render();
            glfwSwapBuffers(window);

            // FPS berechnen und im Fenstertitel anzeigen
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