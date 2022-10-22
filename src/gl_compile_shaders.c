#include "gl_compile_shaders.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **load_source_from_file(const char *shader_file_path) {
    FILE *shader_source_file = fopen(shader_file_path, "r");
    if (!shader_source_file) {
        printf("load_source_from_file: Error openning source file %s\n",
               shader_file_path);
        return NULL;
    }

    size_t line_count = 0;
    for (char c = (char)fgetc(shader_source_file); c != EOF;
         c = (char)fgetc(shader_source_file)) {
        if (c == '\n')
            line_count++;
    }
    rewind(shader_source_file);

    char **shader_source = malloc(sizeof(char *) * (line_count + 1));
    if (!shader_source) {
        printf("load_source_from_file: Error allocating memory for source data "
               "array\n");
        fclose(shader_source_file);
        return NULL;
    }
    size_t i = 0;
    char line[MAX_GSLS_SOURCE_LINE_SIZE];

    while (fgets(line, MAX_GSLS_SOURCE_LINE_SIZE, shader_source_file) != NULL) {
        size_t read_size = strlen(line);
        shader_source[i] = malloc(sizeof(char) * (read_size + 1));
        if (!shader_source[i]) {
            printf("load_source_from_file: Error allocating memory for source "
                   "data string\n");
            for (size_t j = 0; j < i; j++)
                free(shader_source[j]);
            free(shader_source);
            fclose(shader_source_file);
            return NULL;
        }
        memcpy(shader_source[i], line, read_size);
        shader_source[i][read_size] = 0;
        i++;
    }
    shader_source[line_count] = NULL;

    fclose(shader_source_file);

    return shader_source;
}

GLuint load_shader(const char *shader_file_path, GLenum shader_type) {
    GLuint shader = glCreateShader(shader_type);
    if (!shader) {
        printf("load_shader: Error creating shader\n");
        return 0;
    }

    char **shader_source = load_source_from_file(shader_file_path);
    if (!shader_source) {
        printf("load_shader: Couldn't load source from file %s\n",
               shader_file_path);
        glDeleteShader(shader);
        return 0;
    }

    GLsizei n_lines = 0;
    for (; shader_source[n_lines] != NULL; n_lines++)
        ;

    glShaderSource(shader, n_lines, (const char *const *)shader_source, NULL);
    glCompileShader(shader);

    GLint result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (!result) {

        printf("load_shader: Failed to compile shader\n");
        GLint log_len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);
        if (log_len <= 0) {
            printf("load_shader: Couldn't get a log for the failed compilation "
                   ":(\n");
            glDeleteShader(shader);
            for (size_t i = 0; shader_source[i] != NULL; i++)
                free(shader_source[i]);
            free(shader_source);
            return 0;
        }
        char *log_str = malloc(sizeof(GLchar) * (size_t)log_len);
        if (!log_str) {
            printf("load_shader: Failed to allocate space for shader "
                   "log\n");
            glDeleteShader(shader);
            for (size_t i = 0; shader_source[i] != NULL; i++)
                free(shader_source[i]);
            free(shader_source);
            return 0;
        }
        GLsizei end_size;
        glGetShaderInfoLog(shader, log_len, &end_size, log_str);
        printf("load_shader: Compilation log:\n%s", log_str);
        free(log_str);
        glDeleteShader(shader);
        for (size_t i = 0; shader_source[i] != NULL; i++)
            free(shader_source[i]);
        free(shader_source);
        return 0;
    }

    for (size_t i = 0; shader_source[i] != NULL; i++)
        free(shader_source[i]);
    free(shader_source);
    return shader;
}

