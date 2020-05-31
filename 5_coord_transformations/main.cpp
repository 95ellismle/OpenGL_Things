#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

const unsigned int SCR_HEIGHT = 800;
const unsigned int SCR_WIDTH  = 800;

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

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
    GLFWwindow* window = glfwCreateWindow(SCR_HEIGHT, SCR_WIDTH, "Playing With Coords", NULL, NULL);
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
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // Compile the vertex/fragment shader and create the shader program.
    // Create shaders
    shader::SingleShader VertexShader("./src/vertexShader.vert", GL_VERTEX_SHADER); 
    shader::SingleShader FragmentShader("./src/fragmentShader.frag", GL_FRAGMENT_SHADER); 
    shader::SingleShader Shaders[2] = {VertexShader, FragmentShader};
    // Create program
    shader::Program ShaderProgram(Shaders, 2);

    // Create texture Shrek
    unsigned int textureShrek;
    int width, height, nrChannels;
    unsigned char *data;
    glGenTextures(1, &textureShrek);
    glBindTexture(GL_TEXTURE_2D, textureShrek);
    // Set some parameters to tell OpenGL how the texture should be used.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    std::string tex_filepath = "img/shrekface.png";
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(tex_filepath.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
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
    glEnable(GL_DEPTH_TEST);

    // Do the OpenGL infrastructure stuff
    glBindVertexArray(VAO_handle);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_handle);

    glBufferData(GL_ARRAY_BUFFER, Vertices.size, Vertices.data, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Elements.size, Elements.data, GL_STATIC_DRAW);

    // Tell OpenGL where to look for the positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //// Tell OpenGL where to look for the colors
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    //glEnableVertexAttribArray(1);
    // Tell OpenGL where to look for the texture
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 5 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Create the struct to hold the directions
    input::Directions XY;
    XY.y = 0; XY.x = 0;

	// Start the Shader Program
	ShaderProgram.use();
    ShaderProgram.set("textureShrek", 0);

    // Set program variables
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(40.0f),
                                  (float)SCR_WIDTH / (float)SCR_HEIGHT,
                                  0.01f, 100.0f);

	ShaderProgram.set("proj", projection);

    while(!glfwWindowShouldClose(window)) {
        // Process mouse and keyboard events
        input::processInput(window, XY);
        
        // create transformations
        glm::mat4 view       = glm::mat4(1.0f);
        view  = glm::translate(view, glm::vec3(0.0f, 0.0f, XY.y-3.0f));

        // retrieve the matrix uniform locations
		ShaderProgram.set("view", view);

        render::drawFrame();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureShrek);

        glBindVertexArray(VAO_handle);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_handle);

        for (unsigned int i=0; i<10; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            if (i%3 == 0) 
                model = glm::rotate(model, (20*i) + XY.x, glm::vec3(i/10.0f, 3*i/50.0f, 0.5f));
            else
                model = glm::rotate(model, (20*i) + XY.y, glm::vec3(1, 0.3, 0.5));
		    ShaderProgram.set("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


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



