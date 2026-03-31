#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "shader.h"

using namespace std;

// ----------------------------------------------------------------
// Definitions
// ----------------------------------------------------------------

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main() {

    // ----------------------------------------------------------------
    // Initializing stuff
    // ----------------------------------------------------------------

    // Init GLFW (For platform independent API usage)
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Lena ist toll", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Init GLAD (So I won't have to manage different operating system stuff)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { // Load os specific function pointers (glfw returns those)
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    // Shader
    Shader ourShader("shaders/3.3.shader.vs", "shaders/3.3.shader.fs"); 

    // ----------------------------------------------------------------
    // Data and Buffer init
    // ----------------------------------------------------------------
    float vertices[] = {
    // positions         // colors
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
    };      

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // ----------------------------------------------------------------
    // Main loop
    // ----------------------------------------------------------------
    while(!glfwWindowShouldClose(window))
    {
        processInput(window); 

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.use();

        // uniform is vec2
        // should calc offsetX and offsetY based on time, so the triangle moves in a circle
        int offsetPosition = glGetUniformLocation(ourShader.ID, "positionOffset");
        float timeValue = glfwGetTime();
        float offsetX = 0.5f * cos(timeValue);
        float offsetY = 0.5f * sin(timeValue);
        glUniform2f(offsetPosition, offsetX, offsetY);
        
        

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glfwSwapBuffers(window);
        glfwPollEvents();        
    }

    glfwTerminate();
    return 0;
}

// Called everytime, the window gets resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0,0, width, height);
}

// Called every frame - used to react to user input
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}