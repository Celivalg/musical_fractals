#include "render.h"
#include "camera.h"
#include "gl_create_program.h"
#include "gl_init_buffers.h"
#include "mouse_input.h"
#include <stdlib.h>
/*
 *this doesn't work
static float fps = 0.0f;

void calculate_fps(struct context *context) {
    struct timeval time;
    gettimeofday(&time, NULL);
    long inter_s = time.tv_sec - context->gl_context->last_update.tv_sec;
    if (inter_s < 0)
        inter_s = 0;

    long inter_ms =
        (time.tv_usec - context->gl_context->last_update.tv_usec) / 1000;
    if (inter_ms < 0)
        inter_ms = 0;

    long ms = inter_ms + 1000 * inter_s;
    if (ms <= 0)
        ms = 1;
    fps = fps + ((1000.0f / (float)ms) / 60.0f) - (fps / 60.0f);
    printf("%f\n", fps);
}*/

static void force_redraw(struct context *context) {
    // calculate_fps(context);
    gettimeofday(&(context->gl_context->last_update), NULL);
    gtk_widget_queue_draw(GTK_WIDGET(context->gtk_context->area));
}

static gboolean render(GtkGLArea *area,
                       __attribute__((unused)) GdkGLContext *GLcontext,
                       struct context *context) {
    gettimeofday(&(context->gl_context->last_update), NULL);
    // inside this function it's safe to use GL; the given
    // GdkGLContext has been made current to the drawable
    // surface used by the `GtkGLArea` and the viewport has
    // already been set to be the size of the allocation

    calc_camera(context);
    // we can start by clearing the buffer
    glClearColor(0.5, 0.5, 0.5, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    // draw your object
    if (context->gl_context->program != 0 && context->gl_context->VAO != 0) {

        glUseProgram(context->gl_context->program);
        glBindVertexArray(context->gl_context->VAO);

        glUniform2f(context->gl_context->u_resolution_pos,
                    (float)gtk_widget_get_width(GTK_WIDGET(area)),
                    (float)gtk_widget_get_height(GTK_WIDGET(area)));

        glUniform3fv(context->gl_context->u_camera_origin_pos, 1,
                     context->camera->camera_origin);

        glUniform4fv(context->gl_context->u_camera_rotation_q_pos, 1,
                     context->camera->camera_rotation_q);

        glDrawArrays(GL_TRIANGLES, 0, 6);

    } else {
        printf("failed somewhere");
    }

    // we completed our drawing; the draw commands will be
    // flushed at the end of the signal emission chain, and
    // the buffers will be drawn on the window
    struct timeval time;
    gettimeofday(&time, NULL);
    long inter_s = time.tv_sec - context->gl_context->last_update.tv_sec;
    if (inter_s < 0)
        inter_s = 0;

    long inter_ms =
        (time.tv_usec - context->gl_context->last_update.tv_usec) / 1000;
    if (inter_ms < 0)
        inter_ms = 0;

    long ms = (1000 / 60) - inter_ms;

    if (inter_s >= 1 || ms < 0) {
        g_timeout_add(1, G_SOURCE_FUNC(force_redraw), context);
    } else {
        g_timeout_add((unsigned int)ms, G_SOURCE_FUNC(force_redraw), context);
    }

    return TRUE;
}

static void on_realize(GtkGLArea *area, struct context *context) {

    gtk_gl_area_make_current(area);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        printf("on_realize: Failed to init glew\n");
        return;
    }

    if (gtk_gl_area_get_error(area) != NULL) {
        printf("on_realize: gl_area error\n");
        return;
    }

    context->gl_context->VAO = setup_vao();
    if (context->gl_context->VAO == 0) {
        printf("on_realize: Failed to setup VAO\n");
        return;
    }

    context->gl_context->program =
        create_program("./glsl/vertex.glsl", "./glsl/fragment.glsl");
    if (context->gl_context->program == 0) {
        printf("on_realize: Failed to create program\n");
        return;
    }
    glUseProgram(context->gl_context->program);

    context->gl_context->u_resolution_pos =
        glGetUniformLocation(context->gl_context->program, "u_resolution");
    context->gl_context->u_camera_origin_pos =
        glGetUniformLocation(context->gl_context->program, "u_camera_origin");
    context->gl_context->u_camera_rotation_q_pos = glGetUniformLocation(
        context->gl_context->program, "u_camera_rotation_q");
}

GtkWidget *setup_glarea(struct context *context) {
    GtkWidget *gl_area = gtk_gl_area_new();
    context->gtk_context->area = GTK_GL_AREA(gl_area);

    g_signal_connect(gl_area, "render", G_CALLBACK(render), context);
    g_signal_connect(gl_area, "realize", G_CALLBACK(on_realize), context);
    g_timeout_add(1000 / 60, G_SOURCE_FUNC(force_redraw), context);

    gtk_gl_area_set_auto_render(GTK_GL_AREA(gl_area), true);
    return gl_area;
}

