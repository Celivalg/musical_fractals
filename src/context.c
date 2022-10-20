#include "context.h"
#include <stdio.h>
#include <stdlib.h>

void free_context(struct context *context) {
    if (context != NULL) {
        if (context->gl_context != NULL) {
            if (context->camera != NULL) {
                free(context->camera);
                if (context->gtk_context != NULL) {
                    free(context->gtk_context);
                }
            }
            free(context->gl_context);
        }
        free(context);
    }
}

// don't touch until you have read the comments in free_context
struct context *init_context() {
    struct context *context = malloc(sizeof(struct context));
    if (context == NULL) {
        printf("init_context: Failed to allocate context memory\n");
        return NULL;
    }

    context->gl_context = malloc(sizeof(struct gl_context_data));
    if (context->gl_context == NULL) {
        printf("init_context: Failed to allocate context->gl_context "
               "memory\n");
        free_context(context);
        return NULL;
    }

    // Using -2 here as initial values as -1 can be silently ignored
    // sometimes in gl, not that I am catching the errors anyway
    context->gl_context->VAO = 0;
    context->gl_context->program = 0;
    context->gl_context->u_resolution_pos = -2;
    context->gl_context->u_camera_origin_pos = -2;
    context->gl_context->u_camera_rotation_q_pos = -2;

    context->camera = malloc(sizeof(struct camera_data));
    if (context->camera == NULL) {
        printf("init_context: Failed to allocate context->camera memory\n");
        free_context(context);
        return NULL;
    }

    for (int i = 0; i < 3; i++)
        context->camera->camera_origin[i] = 0.0;

    // quaternions are normalized, with q0 (aka q.w, which is last in the
    // vector array) being 1 for no rotation... I'm aware that no rotation
    // doesn't mean anything
    for (int i = 0; i < 3; i++)
        context->camera->camera_rotation_q[i] = 0.0;
    context->camera->camera_rotation_q[3] = 1.0;

    for (int i = 0; i < 3; i++)
        context->camera->camera_vel[i] = 0.0;

    gettimeofday(&(context->camera->last_update), NULL);

    for (int i = 0; i < 6; i++)
        context->camera->pressed[i] = false;

    context->camera->camera_rotation_pitch = 0.0;
    context->camera->camera_rotation_yaw = 0.0;

    context->gtk_context = malloc(sizeof(struct gtk_context_data));
    if (context->gtk_context == NULL) {
        printf(
            "init_context: Failed to allocate context->gtk_context memory\n");
        free_context(context);
        return NULL;
    }
    context->gtk_context->win = NULL;
    context->gtk_context->pointer_grabbed = false;

    return context;
}
