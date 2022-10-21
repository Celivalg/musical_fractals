#ifndef CONTEXT_H
#define CONTEXT_H

#include <GL/glew.h>
#include <gtk/gtk.h>
#include <stdbool.h>
#include <sys/time.h>

enum direction {
    d_forward = 0,
    d_backward = 1,
    d_up = 2,
    d_down = 3,
    d_left = 4,
    d_right = 5
};

struct gl_context_data {
    GLuint VAO;
    GLuint program;
    GLint u_resolution_pos;
    GLint u_camera_origin_pos;
    GLint u_camera_rotation_q_pos;
    struct timeval last_update;
};

struct gtk_context_data {
    bool pointer_grabbed;
    GtkGLArea *area;
    GtkWidget *win;
};

struct camera_data {
    GLfloat camera_origin[3];
    GLfloat camera_rotation_q[4];
    GLfloat camera_rotation_pitch;
    GLfloat camera_rotation_yaw;

    GLfloat camera_vel[3];

    struct timeval last_update;
    bool pressed[6];
};

// gl_context not to be mistaken for an actual gl_context, but just for the data
// I need to run the shaders
struct context {
    struct gl_context_data *gl_context;
    struct camera_data *camera;
    struct gtk_context_data *gtk_context;
};

void free_context(struct context *context);
struct context *init_context();

#endif
