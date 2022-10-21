#include "key_events.h"
#include "mouse_input.h"
#include <stdio.h>

gboolean key_pressed_handler(__attribute__((unused)) GtkEventController *self,
                             guint keyval,
                             __attribute__((unused)) guint keycode,
                             __attribute__((unused)) GdkModifierType state,
                             struct context *context) {
    switch (keyval) {
    case 'Z':
    case 'z':
        if (context->gtk_context->pointer_grabbed)
            key_forward(context, d_forward, true);
        break;
    case 'S':
    case 's':
        if (context->gtk_context->pointer_grabbed)
            key_forward(context, d_backward, true);
        break;
    case 'D':
    case 'd':
        if (context->gtk_context->pointer_grabbed)
            key_forward(context, d_right, true);
        break;
    case 'Q':
    case 'q':
        if (context->gtk_context->pointer_grabbed)
            key_forward(context, d_left, true);
        break;
    case 32: // space
        if (context->gtk_context->pointer_grabbed)
            key_forward(context, d_up, true);
        break;
    case 65505: // shift
        if (context->gtk_context->pointer_grabbed)
            key_forward(context, d_down, true);
        break;
    case 65307: // escape
        if (context->gtk_context->pointer_grabbed) {
            key_forward(context, d_down, false);
            key_forward(context, d_up, false);
            key_forward(context, d_right, false);
            key_forward(context, d_left, false);
            key_forward(context, d_forward, false);
            key_forward(context, d_backward, false);
            ungrab_mouse(context);
        } else {
            grab_mouse(context);
        }
        break;
    default:
        break;
    }

    // printf("keyval: %u\n", keyval);

    return true;
}

void key_released_handler(__attribute__((unused)) GtkEventController *self,
                          guint keyval, __attribute__((unused)) guint keycode,
                          __attribute__((unused)) GdkModifierType state,
                          struct context *context) {
    switch (keyval) {
    case 'Z':
    case 'z':
        key_forward(context, d_forward, false);
        break;
    case 'S':
    case 's':
        key_forward(context, d_backward, false);
        break;
    case 'D':
    case 'd':
        key_forward(context, d_right, false);
        break;
    case 'Q':
    case 'q':
        key_forward(context, d_left, false);
        break;
    case 32: // space
        key_forward(context, d_up, false);
        break;
    case 65505: // shift
        key_forward(context, d_down, false);
        break;
    default:
        break;
    }
}

GtkEventController *new_event_controller(struct context *context) {
    GtkEventController *controller = gtk_event_controller_key_new();
    g_signal_connect(controller, "key-pressed", G_CALLBACK(key_pressed_handler),
                     context);
    g_signal_connect(controller, "key-released",
                     G_CALLBACK(key_released_handler), context);
    return controller;
}
