#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

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

    // Init GLAD (So I won't have to manage different operating system stuff)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { // Load os specific function pointers (glfw returns those)
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    glViewport(0, 0, 800, 600); // Window dimensions

    // ----------------------------------------------------------------
    // Callback functions
    // ----------------------------------------------------------------
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // ----------------------------------------------------------------
    // Main loop
    // ----------------------------------------------------------------
    while(!glfwWindowShouldClose(window))
    {
        processInput(window); 

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
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