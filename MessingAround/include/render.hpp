#ifndef RENDER_HEADER_GUARD
#define RENDER_HEADER_GUARD

#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace render {

    /*
     Will draw the frame we see.
    
     This is a bit like the main function for the rendering and takes care of
     basically everything.
    */
    void drawFrame(const float *background_rgba) {
        // Just fill a solid color
        glClearColor(background_rgba[0], background_rgba[1],
                     background_rgba[2], background_rgba[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}    
    
    
#endif
