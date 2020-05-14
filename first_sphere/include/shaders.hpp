#ifndef SHADERS_HEADER_GUARD
#define SHADERS_HEADER_GUARD

#include <string>
#include <glad/glad.h>

#include <files.hpp>
#include <iostream>

namespace shader {

    /*
     Will load the shader and compile them.

     This will also check for compilation errors and print any.
    */
    class SingleShader {

        private:
            /*
             A little method to check the shader has compiled correctly.

             Will throw an error if not and will print the OpenGL info_log.
            */
            char info_log[512];
            void check_shader_compilation(GLenum shader_type) {
                glGetShaderiv(handle, GL_COMPILE_STATUS, &success);

                if (!success) {
                    glGetShaderInfoLog(handle, 512, NULL, info_log);
                    switch (shader_type) {
                        case GL_FRAGMENT_SHADER:
                            std::cerr << "\n\nVertex Shader Compilation Failed\n";
                            std::cerr << info_log << std::endl;
                            throw "ShaderError";

                        case GL_VERTEX_SHADER:
                            std::cerr << "\n\nFragment Shader Compilation Failed\n";
                            std::cerr << info_log << std::endl;
                            throw "ShaderError";

                        default:
                            std::cerr << "\n\nUnknown Shader Type: " << shader_type << "\nInfoLog: ";
                            std::cerr << info_log << std::endl;
                            throw "ShaderError";
                    }
                }
            }

        protected:

        public:
            int success;
            unsigned int handle;
            const char *shader_program_txt;
        
            /*
                Constructor: Will create the shader and attach the shader txt from a file.

                Inputs:
                    * fp <std::string> => The path of the shader program.
                    * shader_type <GLenum> => The type of shader to compile.
            */
            SingleShader (std::string fp, GLenum shader_type) {
                // Create a shader
                handle = glCreateShader(shader_type);
                
                // Read the shader file
                IO::File shader_file;
                shader_file.read(fp);
                shader_program_txt = shader_file.file_txt.c_str();
                
                // Attach the shader program to the shader
                glShaderSource(handle, 1, &shader_program_txt, NULL);
                glCompileShader(handle);

                // Check the shader for errors
                check_shader_compilation(shader_type);
            }
    };


    /*
     Create a shader program.
    */
    class Program {
        private:
            char info_log[512];
            void check_link_errors() {
                glGetProgramiv(handle, GL_LINK_STATUS, &success);
                if (!success) {
                    glGetProgramInfoLog(handle, 512, NULL, info_log);
                    std::cerr << "\n\nShader Program Link Error:";
                    std::cerr << " Info Log: " << info_log << std::endl;
                    throw "ShaderProgramError";
                }
            }

        protected:

        public:
            unsigned int handle;
            int success;

            Program(SingleShader Shaders[], unsigned int num_shaders) {
                // Create the shader program object
                handle = glCreateProgram();
            
                // Attach the shaders
                for (unsigned int i=0; i<num_shaders; i++) {
                    glAttachShader(handle, Shaders[i].handle);
                }
                glLinkProgram(handle);

                // Check everything went ok
                check_link_errors();

                // Delete shaders
                for (unsigned int i=0; i<num_shaders; i++) {
                    glDeleteShader(Shaders[i].handle);
                }
            };
    };
}

#endif
