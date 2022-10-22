#ifndef GL_LOAD_SHADERS_H
#define GL_LOAD_SHADERS_H

#include "context.h"
#include <gtk/gtk.h>

bool load_shaders(struct context *context);
void load_default_shaders(GtkButton *self, struct context *context);

#endif
