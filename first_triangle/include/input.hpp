#ifndef INPUT_HEADER_GUARD
#define INPUT_HEADER_GUARD

//#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace input {
    /*
     Will process any inputs from the keyboard or mouse.
    */
    void processInput(GLFWwindow *window)
    {
        // If escape is pressed then close the window
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {   
            glfwSetWindowShouldClose(window, true);
        }   
    }
}

#endif
