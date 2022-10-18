#include "render.h"
#include "gl_create_program.h"
#include "gl_init_buffers.h"

static gboolean render(GtkGLArea *area,
                       __attribute__((unused)) GdkGLContext *GLcontext,
                       struct context *context) {
    // inside this function it's safe to use GL; the given
    // GdkGLContext has been made current to the drawable
    // surface used by the `GtkGLArea` and the viewport has
    // already been set to be the size of the allocation

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

    g_signal_connect(gl_area, "render", G_CALLBACK(render), context);
    g_signal_connect(gl_area, "realize", G_CALLBACK(on_realize), context);

    gtk_gl_area_set_auto_render(GTK_GL_AREA(gl_area), true);
    return gl_area;
}

