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
    // Pendel initialisieren und Kreise vorbereiten
    // ------------------------------------------------------------

    double theta1 = 100 * M_PI / 180.0; // TODO: über UI initialisieren 
    double theta2 = -80 * M_PI / 180.0;

    Pendulumjoint joint1 = {1.0, 1.0, theta1, 0.0};
    Pendulumjoint joint2 = {1.0, 1.0, theta2, 0.0};

    float scale = 0.75f / (joint1.length + joint2.length);

    // ------------------------------------------------------------
    // Main Loop
    // ------------------------------------------------------------
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        processInput(window);

        ui.startFrame();
        ui.defineStyleAndUi();

        // Pendel simulieren
        timeStepRK4(joint1, joint2, Config::DT, Config::G);

        double x1, y1, x2, y2;
        getKartesianCoordinates(joint1, joint2, x1, y1, x2, y2);

        float fx1 = static_cast<float>(x1);
        float fy1 = static_cast<float>(y1);
        float fx2 = static_cast<float>(x2);
        float fy2 = static_cast<float>(y2);

        renderer.update(fx1, fy1, fx2, fy2);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderer.draw(scale);

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
