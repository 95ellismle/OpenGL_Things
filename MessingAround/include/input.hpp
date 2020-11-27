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
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        float yaw = 0.0f;
        float pitch = 0.0f;
        float roll = 0.0f;
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
            XY.x=0.0f; XY.y=0.0f; XY.z=0.0f;
            XY.yaw=0.0f; XY.pitch=0.0f; XY.roll=0.0f;
            // Handle directions
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                XY.y = 1.0f;
                to_return = true;
            } 

            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                XY.y = -1.0f;
                to_return = true;
            }

            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                XY.x = -1.0f;
                to_return = true;
            } 

            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
                XY.x = 1.0f;
                to_return = true;
            }

            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
                XY.z = 1.0f;
                to_return = true;
            } 

            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
                XY.z = -1.0f;
                to_return = true;
            }

            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                XY.yaw= 1.0f;
                to_return = true;
            } 

            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                XY.yaw = -1.0f;
                to_return = true;
            }
        }
        return to_return;
    }
}

#endif
