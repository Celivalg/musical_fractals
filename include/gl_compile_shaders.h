#ifndef GL_COMPILE_SHADERS_H
#define GL_COMPILE_SHADERS_H

#include <GL/glew.h>
// truth be told, this could easily be automatically determined at runtime...
// might save a lot of headaches in the long run... but oh look a squirrel TODO
#define MAX_GSLS_SOURCE_LINE_SIZE 4096

// Loads the shader and returns their position in GL memory
// Does handle failed compilation and logs stuff to stdout, not stderr (might
// need to change that TODO)
// returns -1 on error
GLuint load_shader(const char *shader_file_path, GLenum shader_type);

#endif
