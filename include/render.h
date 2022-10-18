#ifndef RENDER_H
#define RENDER_H

#include "context.h"
#include <GL/glew.h>
#include <gtk/gtk.h>

// Inits the GtkGlArea and sets the render and initialize handlers.
// also initializes the memory of 'context->gl_context', but the pointer needs
// to be kept into memory as long as the application is running. No real need to
// free it in the end as the application would stop anyway, or even if the
// GLArea would need to be reinitialized, the 'context' can be reused, tho a lot
// of thing would probably end up staying in memory... might need a TODO here
GtkWidget *setup_glarea(struct context *context);

#endif
