#ifndef GL_CREATE_PROGRAM_H
#define GL_CREATE_PROGRAM_H

#include <GL/glew.h>

// Creates a program, might log stuff to stdout, not stderr (TODO again)
// returns -1 on error
GLuint create_program(const char *vertex_file, const char *fragment_file);

#endif
