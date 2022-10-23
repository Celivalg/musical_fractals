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
    GLint u_max_steps_pos;
    GLint u_max_dist_pos;
    GLint u_surface_tresh_pos;
    GLint u_de_vec_pos;
    struct timeval last_update;
    char *vertex_source;
    char *fragment_source;
};

// could have been done with 3 states but cleaner this way
struct paned_animation_data {
    bool is_closed;
    bool is_open;
    bool is_closing;
    bool is_opening;
    GtkWidget *paned;
};

struct gtk_context_data {
    bool pointer_grabbed;
    GtkGLArea *area;
    GtkWidget *win;
    struct paned_animation_data anim_data;

    GtkRange *vec_x;
    GtkRange *vec_y;
    GtkRange *vec_z;
};

struct drawing_data {
    unsigned int max_step;
    float max_dist;
    float surface_tresh;

    float vec[3];
};

struct camera_data {
    GLfloat camera_origin[3];
    GLfloat camera_rotation_q[4];
    GLfloat camera_rotation_pitch;
    GLfloat camera_rotation_yaw;

    GLfloat camera_vel[3];

    struct timeval last_update;
    bool pressed[6];

    float camera_speed;
    float camera_accel;
    float camera_decel;
    float camera_sensitivity;
};

// gl_context not to be mistaken for an actual gl_context, but just for the data
// I need to run the shaders

// I also need to stop this from using references... gonna be hell to adjust the
// code everywhere but it needs to be done
struct context {
    struct gl_context_data *gl_context;
    struct camera_data *camera;
    struct gtk_context_data *gtk_context;
    struct drawing_data drawing;
};

void free_context(struct context *context);
struct context *init_context();

#endif
