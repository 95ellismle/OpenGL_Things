#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdlib>

// Own modules
#include <input.hpp>
#include <render.hpp>
#include <files.hpp>
#include <shaders.hpp>
#include <cmath>


shader::Program ShaderProgram;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);


unsigned int SCR_HEIGHT = 1000;
unsigned int SCR_WIDTH  = 1000;
const unsigned int numCubes = 10;
const float backgroundRGBA[4] = {0.0, 0.0, 0.0, 0.0};

std::vector<glm::vec3> cubePositions;

//// Init camera at -3z pointing at origin.
//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
//glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
//glm::vec3 viewDirection = glm::normalize(cameraPos - cameraTarget);
//glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
//glm::vec3 cameraRight = glm::normalize(glm::cross(up, viewDirection));
//glm::vec3 cameraUp = glm::normalize(glm::cross(viewDirection, cameraRight));


std::vector<std::vector<float>> randRot;
int main () {
    
    // Allocate random positions for the cubes
    cubePositions.resize(numCubes);
    randRot.resize(numCubes);
    for (unsigned int i=0; i<numCubes; i++) {
        double x = 50 * (-0.5 + (rand() / (double) RAND_MAX));
        double y = 50 * (-0.5 + (rand() / (double) RAND_MAX));
        double z = 50 * (-0.5 + (rand() / (double) RAND_MAX));
        cubePositions[i] = glm::vec3(x, y, z);
        randRot[i].resize(3);
        randRot[i][0] = 10.0f * rand() / RAND_MAX;
        randRot[i][1] = 10.0f * rand() / RAND_MAX;
        randRot[i][2] = 10.0f * rand() / RAND_MAX;
    }
    

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
    GLFWwindow* window = glfwCreateWindow(SCR_HEIGHT, SCR_WIDTH, "Playing With Shrek", NULL, NULL);
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
    ShaderProgram.addShaders(Shaders, 2);

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
    input::Directions Pos_input;
    input::Directions Pos;
    Pos.y = 0.0f; Pos.x = 0.0f; Pos.z = -3.0f;
    Pos_input.y = 0.0f; Pos_input.x = 0.0f; Pos_input.z = 0.0f;

	// Start the Shader Program
	ShaderProgram.use();
    ShaderProgram.set("textureShrek", 0);

    // Set program variables
	ShaderProgram.setPerspective((float) SCR_WIDTH, (float) SCR_HEIGHT);

    float deltaTime = 0.0f;
    float lastTime = 0.0f;
    while(!glfwWindowShouldClose(window)) {
        float currTime = glfwGetTime();
        deltaTime = currTime - lastTime;

        // Process mouse and keyboard events
        input::processInput(window, Pos_input);
        Pos.x = Pos.x + 4*(Pos_input.x * deltaTime);
        Pos.y = Pos.y + 4*(Pos_input.y * deltaTime);
        Pos.z = Pos.z + 4*(Pos_input.z * deltaTime);
        
        // create transformations
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(Pos.x, Pos.y, Pos.z));

        //glm::vec3 direction;
        //direction.x = cos(glm::radians(yaw));
        //direction.z = sin(glm::radians(yaw));
        //cameraFront = glm::normalize(direction);

        // retrieve the matrix uniform locations
		ShaderProgram.set("view", view);

        render::drawFrame(backgroundRGBA);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureShrek);

        glBindVertexArray(VAO_handle);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_handle);

        for (unsigned int i=0; i<numCubes; i++) {

            
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float time = glfwGetTime();
            model = glm::rotate(model, (float) time * randRot[i][0],
                    glm::vec3(randRot[i][0], randRot[i][1], randRot[i][2]));
            //else
            //    model = glm::rotate(model, (20*i) + Pos.y, glm::vec3(1, 0.3, 0.5));
		    ShaderProgram.set("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        glfwSwapBuffers(window); // Swap the 2D image front and back buffers
        glfwPollEvents(); // Check for any mouse or keyboard events

        lastTime = currTime;
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



void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    glScissor(0, 0, width, height);

    ShaderProgram.setPerspective((float) width, (float) height);
}
