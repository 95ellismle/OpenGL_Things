#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Own modules
#include <input.hpp>
#include <render.hpp>
#include <files.hpp>
#include <shaders.hpp>

float vertices[] = {
     0.5,  0.5, 0.0,  // top right
     0.5, -0.5, 0.0,  // bottom right
    -0.5, -0.5, 0.0,  // bottom left
    -0.5,  0.5, 0.0   // top left 
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
}; 



const char *vertexShaderSource, *fragShaderSource;

int main () {
    
    // Read the vertices array
    IO::IntArrayFile Elements;
    Elements.read("./data/elements.arr");
    IO::FloatArrayFile Vertices;
    Vertices.read("./data/vertices.arr");
    
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


    // Create the buffers (vertex buffer, vertex array and element buffer)
    unsigned int VBO_handle, VAO_handle, EBO_handle;
    glGenVertexArrays(1, &VAO_handle);
    glGenBuffers(1, &VBO_handle);
    glGenBuffers(1, &EBO_handle);

    // Do the OpenGL infrastructure stuff
    glBindVertexArray(VAO_handle);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_handle);

    glBufferData(GL_ARRAY_BUFFER, Vertices.size, Vertices.data, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Elements.size, Elements.data, GL_STATIC_DRAW);

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
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_handle);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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



