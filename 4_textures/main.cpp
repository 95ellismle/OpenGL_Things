#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "stb_image.h"
#include <glm/glm.hpp>

// Own modules
#include <input.hpp>
#include <render.hpp>
#include <files.hpp>
#include <shaders.hpp>
#include <cmath>


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

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
    GLFWwindow* window = glfwCreateWindow(800, 600, "Playing With Textures", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    } glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

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

    // Create texture 1
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // Set some parameters to tell OpenGL how the texture should be used.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load the image
    int width, height, nrChannels;
    std::string tex_filepath = "img/container.jpg";
    unsigned char *data = stbi_load(tex_filepath.c_str(), &width, &height, &nrChannels, 0);
    if (data)  {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture: '" << tex_filepath << "'" << std::endl;
        throw "IOError";
    }
    stbi_image_free(data);

    // Create texture 2
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    tex_filepath = "img/awesomeface.png";
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(tex_filepath.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture: '" << tex_filepath << "' " << std::endl;
        throw "IOError";
    }
    stbi_image_free(data);



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

    // Tell OpenGL where to look for the positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Tell OpenGL where to look for the colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    // Tell OpenGL where to look for the texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void*)(6* sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    int posIncrLocation = glGetUniformLocation(ShaderProgram.handle, "posIncr");
    
    ShaderProgram.use();
    ShaderProgram.set("texture1", 0);
    ShaderProgram.set("texture2", 1);

    // Create the struct to hold the directions
    input::Directions XY;
    XY.y = 0; XY.x = 0;

    // Keep the window open
    while(!glfwWindowShouldClose(window)) {
        // Process mouse and keyboard events
        input::processInput(window, XY);
        
        render::drawFrame();

        glUniform3f(posIncrLocation, XY.x, XY.y, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

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



