#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Own modules
#include <input.hpp>
#include <render.hpp>
#include <files.hpp>
#include <shaders.hpp>


float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

const char *vertexShaderSource, *fragShaderSource;

int main () {
    
    /*
     Init methods -initialise GLAD and GLFW and check everything is linked properly.
    */
    // Check GLFW is initialised correctly
    if (!glfwInit()) {
        std::cerr << "GLFW not configured correctly!" << std::endl;
        return -1;
    }

    // Create the context for the GLFW window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create the window
    GLFWwindow* window = glfwCreateWindow(800, 600, "First Triangle", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    } glfwMakeContextCurrent(window);

    // Check Glad is initialised correctly
    //  This must be after the glfwMakeContextCurrent is called.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialise GLAD" << std::endl;
        return -1;
    }

    

    /*
     OpenGL Stuff -Creating a triangle and rendering
    */
    // Create the OpenGL canvas and make it resize when the window is resized
    glViewport(0, 0, 800, 600);

    // Compile the vertex/fragment shader and create the shader program.
    // Create shaders
    shader::SingleShader VertexShader("./src/vertexShader.vert", GL_VERTEX_SHADER); 
    shader::SingleShader FragmentShader("./src/fragmentShader.frag", GL_FRAGMENT_SHADER); 
    shader::SingleShader Shaders[2] = {VertexShader, FragmentShader};
    // Create program
    shader::Program ShaderProgram(Shaders, 2);


    // Create the vertex buffer
    unsigned int VBO_handle, VAO_handle;
    glGenVertexArrays(1, &VAO_handle);
    glGenBuffers(1, &VBO_handle);

    glBindVertexArray(VAO_handle);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_handle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    // Keep the window open
    while(!glfwWindowShouldClose(window)) {
        // Process mouse and keyboard events
        input::processInput(window);
    
        render::drawFrame();
        glUseProgram(ShaderProgram.handle);
        glBindVertexArray(VAO_handle);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window); // Swap the 2D image front and back buffers
        glfwPollEvents(); // Check for any mouse or keyboard events
    }



    /*
     Finalise -deallocate and tidy up memory
    */
    glDeleteVertexArrays(1, &VAO_handle);
    glDeleteBuffers(1, &VBO_handle);
    glDeleteProgram(ShaderProgram.handle);
    glfwTerminate();

    return 0;
}



