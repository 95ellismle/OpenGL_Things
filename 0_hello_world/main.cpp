#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>



int main () {
    // Check GLFW is initialised correctly
    if (!glfwInit())
    {
        std::cerr << "GLFW not configured correctly!" << std::endl;
        return -1;
    } else {
        std::cout << "Hello World, GLFW Works!" << std::endl;
    }

    // Create the context for the GLFW window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create the window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    } glfwMakeContextCurrent(window);

    // Check Glad is initialised correctly
    //  This must be after the glfwMakeContextCurrent is called.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialise GLAD" << std::endl;
        return -1;
    }

    // Create the OpenGL canvas and make it resize when the window is resized
    glViewport(0, 0, 800, 600);

    // Keep the window open
    while(!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window); // Swap the 2D image front and back buffers
        glfwPollEvents(); // Check for any mouse or keyboard events
    }

    glfwTerminate();
    return 0;
}
