#ifndef INPUT_HEADER_GUARD
#define INPUT_HEADER_GUARD

//#include <glad/glad.h>
#include <iostream>
#include <GLFW/glfw3.h>


namespace input {

    /*
     A struct to hold x and y directions
    */
    struct Directions {
        float x;
        float y;
    };

    /*
     Will process any inputs from the keyboard or mouse.
    */
    bool processInput(GLFWwindow *window, Directions &XY)
    {
        bool to_return = false;
        // If escape is pressed then close the window
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {   
            glfwSetWindowShouldClose(window, true);
            to_return = true;
        }

        // Handle all other keys
        else {
            // Handle directions
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
                XY.y = XY.y + 0.01;
                to_return = true;
            } 

            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
                XY.y = XY.y - 0.01;
                to_return = true;
            }

            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
                XY.x = XY.x + 0.01;
                to_return = true;
            } 

            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                XY.x = XY.x - 0.01;
                to_return = true;
            }
        }
        return to_return;
    }
}

#endif
