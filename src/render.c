#include "render.h"
#include "gl_create_program.h"
#include "gl_init_buffers.h"

static gboolean render(__attribute__((unused)) GtkGLArea *area,
                       __attribute__((unused)) GdkGLContext *context,
                       struct gl_context_data *data) {
    // inside this function it's safe to use GL; the given
    // GdkGLContext has been made current to the drawable
    // surface used by the `GtkGLArea` and the viewport has
    // already been set to be the size of the allocation

    // we can start by clearing the buffer
    glClearColor(0.5, 0.5, 0.5, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw your object
    if (data->program != 0 && data->VAO != 0) {
        GdkSurface *surface = gdk_gl_context_get_surface(context);
        glUseProgram(data->program);
        glBindVertexArray(data->VAO);
        glUniform2f(data->u_resolution_pos,
                    (float)gdk_surface_get_width(surface),
                    (float)gdk_surface_get_height(surface));
        glUniform3f(data->u_camera_origin_pos, 0.0, 0.0, 0.0);
        glUniform4f(data->u_camera_rotation_q_pos, 0.0, 0.0, 0.0, 1.0);
        // glUniform1f(u_time_pos, 2.0f);

        glDrawArrays(GL_TRIANGLES, 0, 6);
    } else {
        printf("failed somewhere");
    }

    // we completed our drawing; the draw commands will be
    // flushed at the end of the signal emission chain, and
    // the buffers will be drawn on the window
    return TRUE;
}

static void on_realize(GtkGLArea *area, struct gl_context_data *data) {

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

    data->VAO = setup_vao();
    if (data->VAO == 0) {
        printf("on_realize: Failed to setup VAO\n");
        return;
    }

    data->program =
        create_program("./glsl/vertex.glsl", "./glsl/fragment.glsl");
    if (data->program == 0) {
        printf("on_realize: Failed to create program\n");
        return;
    }
    glUseProgram(data->program);

    // u_time_pos = glGetUniformLocation(program, "u_time");
    data->u_resolution_pos =
        glGetUniformLocation(data->program, "u_resolution");
    data->u_camera_origin_pos =
        glGetUniformLocation(data->program, "u_camera_origin");
    data->u_camera_rotation_q_pos =
        glGetUniformLocation(data->program, "u_camera_rotation_q");
}

GtkWidget *setup_glarea(struct gl_context_data *data) {
    GtkWidget *gl_area = gtk_gl_area_new();

    g_signal_connect(gl_area, "render", G_CALLBACK(render), data);
    g_signal_connect(gl_area, "realize", G_CALLBACK(on_realize), data);
    return gl_area;
}

