#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <vector>

#include "shader.h"
#include "calculatePendulum.h"

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

    GLFWwindow* window = glfwCreateWindow(800, 600, "Doppelpendel", NULL, NULL);
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

    Shader customShader("shaders/3.3.shader.vs", "shaders/3.3.shader.fs");

    // ------------------------------------------------------------
    // Pendel initialisieren und Kreise vorbereiten
    // ------------------------------------------------------------

    double theta1 = 180 * M_PI / 180.0;
    double theta2 = 80 * M_PI / 180.0;

    Pendulumjoint joint1 = {1.0, 1.0, theta1, 0.0};
    Pendulumjoint joint2 = {1.0, 1.0, theta2, 0.0};

    float scale = 0.75f / (joint1.length + joint2.length);

    float vertices[4 * 6];

    float radius = 0.03f;

    const int N = 64;
    const int VERTS_PER_CIRCLE = 1 + (N + 1);           // mitte + ring(+1)
    vector<float> circles(3 * (3 * VERTS_PER_CIRCLE)); // 3 Kreise

    auto fillDisk = [&](int circleIndex, float cx, float cy) {
        int startVertex = circleIndex * VERTS_PER_CIRCLE;
        int startFloat  = 3 * startVertex;

        // mitte
        circles[startFloat + 0] = cx;
        circles[startFloat + 1] = cy;
        circles[startFloat + 2] = 0.0f;

        // ring
        for (int i = 0; i <= N; ++i) {
            float a = 2.0f * (float)M_PI * (float)i / (float)N;
            circles[startFloat + 3*(1+i) + 0] = cx + radius * cos(a);
            circles[startFloat + 3*(1+i) + 1] = cy + radius * sin(a);
            circles[startFloat + 3*(1+i) + 2] = 0.0f;
        }
    };

    // ------------------------------------------------------------
    // Leere Buffer
    // ------------------------------------------------------------

    // Linien
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), NULL, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Kreise
    unsigned int circleVAO, circleVBO;

    glGenVertexArrays(1, &circleVAO);
    glGenBuffers(1, &circleVBO);

    glBindVertexArray(circleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
    glBufferData(GL_ARRAY_BUFFER, circles.size() * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // ------------------------------------------------------------
    // Konstanten und Variablen
    // ------------------------------------------------------------

    double dt = 0.001;
    double g = 9.81;

    // ------------------------------------------------------------
    // Main Loop
    // ------------------------------------------------------------
    while (!glfwWindowShouldClose(window))
    {
        // Pendel simulieren
        timeStepRK4(joint1, joint2, dt, g);

        double x1, y1, x2, y2;
        getKartesianCoordinates(joint1, joint2, x1, y1, x2, y2);

        float fx1 = static_cast<float>(x1);
        float fy1 = static_cast<float>(y1);
        float fx2 = static_cast<float>(x2);
        float fy2 = static_cast<float>(y2);

        // Vertices der linien neu setzen
        float newVertices[] = {
            // x, y, z
            0.0f, 0.0f, 0.0f,
            fx1,  fy1,  0.0f,

            fx1,  fy1,  0.0f,
            fx2,  fy2,  0.0f
        };

        // Kreise neu setzen
        fillDisk(0, 0.0f, 0.0f);
        fillDisk(1, fx1,  fy1);
        fillDisk(2, fx2,  fy2);

        // VBO updaten
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(newVertices), newVertices);

        glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, circles.size() * sizeof(float), circles.data());

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        customShader.use();
        // Linie
        customShader.setFloat("scale", scale);
        customShader.setVec2("offset", 0.0f, 0.0f);

        glBindVertexArray(VAO);
        customShader.setVec4("uColor", 0.8f, 0.2f, 0.6f, 1.0f); 
        glDrawArrays(GL_LINES, 0, 4);
        
        // farbe setzen Kreise
        customShader.setVec4("uColor", 1.0f, 0.2f, 0.2f, 1.0f); 
        glBindVertexArray(circleVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0 * VERTS_PER_CIRCLE, VERTS_PER_CIRCLE);
        glDrawArrays(GL_TRIANGLE_FAN, 1 * VERTS_PER_CIRCLE, VERTS_PER_CIRCLE);
        glDrawArrays(GL_TRIANGLE_FAN, 2 * VERTS_PER_CIRCLE, VERTS_PER_CIRCLE);

        glfwSwapBuffers(window);
        glfwPollEvents();
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
