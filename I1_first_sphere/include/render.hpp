#ifndef RENDER_HEADER_GUARD
#define RENDER_HEADER_GUARD

#include <glad/glad.h>
//#include <GLFW/glfw3.h>


namespace render {

    /*
     Will draw the frame we see.
    
     This is a bit like the main function for the rendering and takes care of
     basically everything.
    */
    void drawFrame()
    {
        // Just fill a solid color
        glClearColor(1.0f, 0.5f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    

}    
    
    
#endif
