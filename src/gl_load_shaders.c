#include "gl_load_shaders.h"
#include "gl_create_program.h"
#include <GL/glew.h>
#include <stdbool.h>

bool load_shaders(struct context *context) {
    // if (context->gl_context->shader_update == false) {
    //     return true;
    // }

    GLuint program = create_program(context->gl_context->vertex_source,
                                    context->gl_context->fragment_source);
    if (program == 0) {
        printf("load_shaders: failed to load program with files %s, %s\n",
               context->gl_context->vertex_source,
               context->gl_context->fragment_source);
        return false;
    }
    GLuint old_program = context->gl_context->program;
    glDeleteProgram(old_program);
    context->gl_context->program = program;
    glUseProgram(context->gl_context->program);

    context->gl_context->u_resolution_pos =
        glGetUniformLocation(context->gl_context->program, "u_resolution");
    context->gl_context->u_camera_origin_pos =
        glGetUniformLocation(context->gl_context->program, "u_camera_origin");
    context->gl_context->u_camera_rotation_q_pos = glGetUniformLocation(
        context->gl_context->program, "u_camera_rotation_q");
    context->gl_context->u_max_steps_pos =
        glGetUniformLocation(context->gl_context->program, "u_max_steps");
    context->gl_context->u_max_dist_pos =
        glGetUniformLocation(context->gl_context->program, "u_max_dist");
    context->gl_context->u_surface_tresh_pos =
        glGetUniformLocation(context->gl_context->program, "u_surface_tresh");
    context->gl_context->u_de_vec_pos =
        glGetUniformLocation(context->gl_context->program, "u_de_vec");

    return true;
}

void load_default_shaders(__attribute__((unused)) GtkButton *self,
                          struct context *context) {
    load_shaders(context);
}
