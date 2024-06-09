#include <stdlib.h>
#include <sys/stat.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "logman/logman.h"

#include "ssp_helper.h"
#include "ssp_shader.h"
#include "ssp_memory.h"

static struct ssp_shader_t{
    GLuint program_id;

    char* vertex;
    char* fragment;
} ssp_shader;

static int ssp_shader_read(const char* shader_path, char** shader)
{
    FILE* shader_file = fopen(shader_path, "r");
    if (shader_file == NULL) {
        log_error("Couldn't open shader file: %s", shader_path);
        return 1;
    }

    struct stat sbuff = { 0 };
    if (stat(shader_path, &sbuff) != 0) {
        log_error("Couldn't stat file: %s", shader_path);
        return 1;
    }

    *shader = (char*)ssp_malloc(sbuff.st_size * sizeof(char));
    if (*shader == NULL) {
        log_error("Failed to allocate %ld bytes for the shader", sbuff.st_size);
        return 1;
    }

    if (fread(*shader, sizeof(char), sbuff.st_size, shader_file) == 0) {
        log_error("File %s is empty", shader_path);
        return 1;
    }

    return 0;
}

static void ssp_shader_destructor(void)
{
    // if (ssp_shader.fragment != NULL) {
    //     ssp_free(ssp_shader.fragment);
    // }

    // if (ssp_shader.vertex != NULL) {
    //     ssp_free(ssp_shader.vertex);
    // }
}

ssp_static GLuint ssp_shader_create(GLuint shader_type, char** shader_source)
{
    GLuint shader_id = glCreateShader(shader_type);
    glShaderSource(shader_id, 1, (const GLchar**)shader_source, NULL);
    glCompileShader(shader_id);

    GLint success;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if(!success) {
        GLchar infoLog[128];
        glGetShaderInfoLog(shader_id, sizeof(infoLog), NULL, infoLog);
        log_error("Shader compilation error: %s", infoLog);
        return 0;
    }

    return shader_id;
}

static int ssp_shader_compile(GLuint shader_type, char* shader_data)
{
    char* shader_source = NULL;
    switch (shader_type) {
        case GL_VERTEX_SHADER:
            ssp_shader.vertex = shader_data;
            shader_source = ssp_shader.vertex;
            break;
        case GL_FRAGMENT_SHADER:
            ssp_shader.fragment = shader_data;
            shader_source = ssp_shader.fragment;
            break;
        default:
            log_error("Unknown shader type %u", shader_type);
            return 1;
    }

    // if (ssp_shader_read(shader_path, &shader_source) != 0) {
    //     return 0;
    // };

    return ssp_shader_create(shader_type, &shader_source);
}

GLuint ssp_shader_get_program(void)
{
    return ssp_shader.program_id;
}

void ssp_shader_use_program(void)
{
    glUseProgram(ssp_shader.program_id);
}

int ssp_shader_create_program(ssp_shader_info* shaders, int shaders_count)
{
    ssp_shader.program_id = glCreateProgram();
    GLuint shaders_id[shaders_count];

    for (int i = 0; i < shaders_count; i++) {
        shaders_id[i] = ssp_shader_compile(shaders[i].type, shaders[i].data);
        if (shaders_id[i] == 0) {
            log_error("Shader creation error: %s", shaders[i].path);
            ssp_shader_destructor();
            return 0;
        }
        glAttachShader(ssp_shader.program_id, shaders_id[i]);
    }

    GLint success;
    glLinkProgram(ssp_shader.program_id);
    glGetProgramiv(ssp_shader.program_id, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[128];
        glGetProgramInfoLog(ssp_shader.program_id, sizeof(infoLog), NULL, infoLog);
        log_error("Gl program link error: %s", infoLog);
        ssp_shader_destructor();
        return 0;
    }

    for (int i = 0; i < shaders_count; i++) {
        glDeleteShader(shaders_id[i]);
    }

    return ssp_shader.program_id;
}