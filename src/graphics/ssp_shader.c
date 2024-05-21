#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "logman/logman.h"

#include "ssp_shader.h"

static struct {
    GLuint program_id;

    char* vertex;
    char* fragment;
} shader;

static int shader_read(const char* shader_path, char** sh)
{
    FILE* shader_file = fopen(shader_path, "r");
    if (shader_file == NULL) {
        log_error("couldn't open vertex file: %s\n", shader_path);
        return 1;
    }

    struct stat sbuff = { 0 };
    if (stat(shader_path, &sbuff) != 0) {
        log_error("couldn't stat file: %s\n", shader_path);
        return 1;
    }

    *sh = (char*)malloc(sbuff.st_size);
    if (*sh == NULL) {
        log_error("couldn't alloc %ld bytes\n", sbuff.st_size);
        return 1;
    }

    if (fread(*sh, sizeof(char), sbuff.st_size, shader_file) == 0) {
        log_error("file %s is empty", shader_path);
        return 1;
    }

    return 0;
}

static void shader_destructor(void)
{
    if (shader.fragment != NULL) {
        free(shader.fragment);
    }

    if (shader.vertex != NULL) {
        free(shader.vertex);
    }
}

static GLuint shader_create(GLuint shader_type, char** shader_source)
{
    GLuint shader_id = glCreateShader(shader_type);
    glShaderSource(shader_id, 1, (const GLchar**)shader_source, NULL);
    glCompileShader(shader_id);
    GLint success;
    GLchar infoLog[128];
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shader_id, sizeof(infoLog), NULL, infoLog);
        log_error("shader compilation error: %s\n", infoLog);
        return 0;
    }

    return shader_id;
}

static int shader_compile(GLuint shader_type, const char* shader_path)
{
    char* shader_source = NULL;
    switch (shader_type) {
        case GL_VERTEX_SHADER:
            shader_source = shader.vertex;
            break;
        case GL_FRAGMENT_SHADER:
            shader_source = shader.fragment;
            break;
        default:
            log_error("unknown shader type %u\n", shader_type);
            return 1;
    }

    if (shader_read(shader_path, &shader_source) != 0) {
        return 0;
    };

    GLuint id = shader_create(shader_type, &shader_source);
    if (id == 0) {
        return 0;
    }

    return id;
}

GLuint shader_get_program(void)
{
    return shader.program_id;
}

void shader_use_program(void)
{
    glUseProgram(shader.program_id);
}

int shader_create_program(shader_meta* shaders, int shaders_count)
{
    shader.program_id = glCreateProgram();
    GLuint shaders_id[shaders_count];

    for (int i = 0; i < shaders_count; i++) {
        shaders_id[i] = shader_compile(shaders[i].type, shaders[i].path);
        if (shaders_id[i] == 0) {
            log_error("shader compile error: %s\n", shaders[i].path);
            shader_destructor();
            return 0;
        }

        glAttachShader(shader.program_id, shaders_id[i]);
    }

    GLint success;
    GLchar infoLog[128];
    glLinkProgram(shader.program_id);
    glGetProgramiv(shader.program_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader.program_id, sizeof(infoLog), NULL, infoLog);
        log_error("program link error: %s\n", infoLog);
        shader_destructor();
        return 0;
    }

    for (int i = 0; i < shaders_count; i++) {
        glDeleteShader(shaders_id[i]);
    }

    return shader.program_id;
}