#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <vector>

#include "shader.h"
#include "calculatePendulum.h"
#include "config.h"
#include "renderer.h"
#include "ui.h"

using namespace std;

// ----------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void getAndUpdatePendulumParameters(Pendulumjoint &joint1, Pendulumjoint &joint2, UI::parameters& params);
void restartSimulation(Pendulumjoint &joint1, Pendulumjoint &joint2, UI::parameters& params );

int main() {

    // ------------------------------------------------------------
    // OpenGL Kontext erstellen und initialisieren
    // ------------------------------------------------------------

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, Config::WINDOW_TITLE, NULL, NULL);
    if (!window) {
        cout << "Failed to create window" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    Shader customShader(Config::VERTEX_SHADER, Config::FRAGMENT_SHADER);
    Renderer renderer(customShader);
    UI ui(window);

    // ------------------------------------------------------------
    // Pendel initialisieren
    // ------------------------------------------------------------

    Pendulumjoint joint1 = {1.0, 1.0, 0.0, 0.0};
    Pendulumjoint joint2 = {1.0, 1.0, 0.0, 0.0};

    float scale = 0.75f / (joint1.length + joint2.length);
    float x1, y1, x2, y2;

    getAndUpdatePendulumParameters(joint1, joint2, ui.getParams());

    // ------------------------------------------------------------
    // Main Loop
    // ------------------------------------------------------------
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        processInput(window);

        ui.startFrame();
        ui.defineStyleAndUi();

        // Get current parameters from UI
        UI::parameters& params = ui.getParams();

        if (params.traceLengthChanged) {
            renderer.resetRenderer(params.traceLength); 
            params.traceLengthChanged = false;
        }

        getAndUpdatePendulumParameters(joint1, joint2, params);
        scale = 0.75f / (joint1.length + joint2.length);
        renderer.setCircleMultiplier(params.mass1 * 0.5f, params.mass2 * 0.5f);


        if (!params.run) {
            restartSimulation(joint1, joint2, params);
            renderer.resetRenderer(params.traceLength); 
        }

        // Pendel simulieren
        if (params.run) {
            timeStepRK4(joint1, joint2, Config::DT, params.gravity);
        }

        getKartesianCoordinates(joint1, joint2, x1, y1, x2, y2);

        renderer.update(x1, y1, x2, y2);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderer.draw(scale, params.showTrace);

        ui.render();

        glfwSwapBuffers(window);
    }

    glfwTerminate();
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

void getAndUpdatePendulumParameters(Pendulumjoint &joint1, Pendulumjoint &joint2, UI::parameters& params) {
    joint1.length = params.length1;
    joint1.mass = params.mass1;

    joint2.length = params.length2;
    joint2.mass = params.mass2;
}

void restartSimulation(Pendulumjoint &joint1, Pendulumjoint &joint2, UI::parameters& params) {
    joint1.theta = params.angle1 * M_PI / 180.0;
    joint2.theta = params.angle2 * M_PI / 180.0;
    joint1.omega = 0.0;
    joint2.omega = 0.0;
}