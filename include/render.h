#ifndef RENDER_H
#define RENDER_H

#include <GL/glew.h>
#include <gtk/gtk.h>

// Data to keep in memory between renders, might be able to ditch it from here
// somehow, but not a big problem atm, and might be cleaner to do it like that
// anyway
// also no I am not typedefing that shit, it's only gonna be referenced 5 times,
// not worth the headaches that comes with typedefing random stuff, fight me
struct gl_context_data {
    GLuint VAO;
    GLuint program;
    GLint u_resolution_pos;
    GLint u_camera_origin_pos;
    GLint u_camera_rotation_q_pos;
};

// Inits the GtkGlArea and sets the render and initialize handlers.
// also initializes the memory of 'data', but the pointer needs to be kept into
// memory as long as the application is running. No real need to free it in the
// end as the application would stop anyway, or even if the GLArea would need to
// be reinitialized, the 'data' can be reused, tho a lot of thing would probably
// end up staying in memory... might need a TODO here
GtkWidget *setup_glarea(struct gl_context_data *data);

#endif
