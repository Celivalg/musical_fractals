#include "gl_init_buffers.h"

GLuint setup_vao() {
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    const GLfloat vertex_data[] = {
        -1.0f, -1.0f, 0.0f, 1.0f,  -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, -1.0f, 1.0f,  0.0f, 1.0f, 1.0f, 0.0f,
    };
    // yes that VBO postion is lost in the confines of the heap...
    // don't really care tho as the VAO stores the attributes
    // might even be able to get it back through that manually, tho I don't
    // think that would be needed... worst comes to worst, I can always return a
    // tuple.. HA
    GLuint VBO;
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data,
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return VAO;
}
