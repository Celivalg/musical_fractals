#ifndef CONTEXT_H
#define CONTEXT_H

#include <GL/glew.h>

struct gl_context_data {
    GLuint VAO;
    GLuint program;
    GLint u_resolution_pos;
    GLint u_camera_origin_pos;
    GLint u_camera_rotation_q_pos;
};

struct camera_data {
    GLfloat *camera_origin;
    GLfloat *camera_rotation_q;
    GLfloat camera_rotation_pitch;
    GLfloat camera_rotation_yaw;
};

// gl_context not to be mistaken for an actual gl_context, but just for the data
// I need to run the shaders
struct context {
    struct gl_context_data *gl_context;
    struct camera_data *camera;
};

void free_context(struct context *context);
struct context *init_context();

#endif
