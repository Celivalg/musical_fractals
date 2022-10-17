#include "gl_create_program.h"
#include "gl_compile_shaders.h"

#include <GL/glew.h>
#include <gtk/gtk.h>

void _log_program(GLuint program) {
    GLint log_len;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_len);
    if (log_len <= 0) {
        return;
    }
    char *log_str = malloc(sizeof(GLchar) * (size_t)log_len);
    if (!log_str) {
        printf("create_program: Failed to allocate space for program log\n");
        return;
    }
    GLsizei end_size;
    glGetProgramInfoLog(program, log_len, &end_size, log_str);
    printf("create_program: Program log:%s\n", log_str);
    free(log_str);
}

GLuint create_program(const char *vertex_file, const char *fragment_file) {
    GLuint vertex_shader = load_shader(vertex_file, GL_VERTEX_SHADER);
    if (!vertex_shader) {
        printf("create_program: Failed to compile vertex shader\n");
        return 0;
    }
    GLuint fragment_shader = load_shader(fragment_file, GL_FRAGMENT_SHADER);
    if (!fragment_shader) {
        printf("create_program: Failed to compile fragment shader\n");
        glDeleteShader(vertex_shader);
        return 0;
    }

    GLuint program = glCreateProgram();
    if (!program) {
        printf("create_program: Failed to create program\n");
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        return 0;
    }

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    // TODO bind color output to index 0, default back buffer behavior is to
    // grab color from index 0 if I understand corretly, and while FragColor is
    // not bound as 0 here, it still gets attributed 0 since it's the only
    // output, however, it's undefined behavior as far as OpenGL defined.
    // In theory something like glBindFragDataLocation(program, 0, "FragColor");
    // should work, maybe, need to read more man pages
    // Or I can perhaps just hardcode it into the glsl code...
    glLinkProgram(program);

    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status) {
        printf("create_program: Failed to link program\n");
        // linking failed
        _log_program(program);
        glDeleteProgram(program);
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        return 0;
    }

    glValidateProgram(program);
    glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
    if (!status) {
        printf("create_program: program not validated\n");
        _log_program(program);
    } else {
        printf("create_program: program validated\n");
    }

    glDetachShader(program, vertex_shader);
    glDetachShader(program, fragment_shader);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    return program;
}
