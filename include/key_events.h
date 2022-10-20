#ifndef KEY_EVENTS_H
#define KEY_EVENTS_H

#include "camera.h"
#include "context.h"
#include <gtk/gtk.h>

GtkEventController *new_event_controller(struct context *context);

gboolean key_pressed_handler(GtkEventController *self, guint keyval,
                             guint keycode, GdkModifierType state,
                             struct context *context);

#endif
