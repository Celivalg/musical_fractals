#ifndef GL_INIT_BUFFERS_H
#define GL_INIT_BUFFERS_H

#include <GL/glew.h>

// bunch of hardcoded data in there, tho what is currently hardcoded probably
// won't actually ever need to be changed as this is only to create a drawing
// area that fills the whole GLArea, onto which I can draw my pixels... yeah
// compute shaders might have been cleaner, but from what I understand, it's
// actually faster or at least as fast to go through the fragment shader to draw
// random shit if I'm going to print it on the screen anyway
GLuint setup_vao();

#endif
