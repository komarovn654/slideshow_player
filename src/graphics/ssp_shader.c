#include <stdlib.h>
#include <sys/stat.h>

#include "ssp_gl.h"
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
        ssp_syslog(LOG_ERR, "SSP. Couldn't open the shader file: %s", shader_path);
        return 1;
    }

    struct stat sbuff = { 0 };
    if (stat(shader_path, &sbuff) != 0) {
        ssp_syslog(LOG_ERR, "SSP. Couldn't stat the file: %s", shader_path);
        return 2;
    }

    *shader = (char*)ssp_malloc(sbuff.st_size * sizeof(char));
    if (*shader == NULL) {
        ssp_syslog(LOG_ERR, "SSP. Failed to allocate %lld bytes for the shader", sbuff.st_size);
        return 3;
    }

    if (fread(*shader, sizeof(char), sbuff.st_size, shader_file) == 0) {
        ssp_syslog(LOG_ERR, "SSP. File %s is empty", shader_path);
        return 4;
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
    GLuint shader_id = ssp_gl_create_shader(shader_type);
    ssp_gl_shader_source(shader_id, 1, (const GLchar**)shader_source, NULL);
    ssp_gl_compile_shader(shader_id);

    GLint success;
    ssp_gl_get_shaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if(!success) {
        GLchar infoLog[128];
        ssp_gl_get_shader_info_log(shader_id, sizeof(infoLog), NULL, infoLog);
        ssp_syslog(LOG_ERR, "SSP. Shader compilation error: %s", infoLog);
        return 0;
    }

    char shader_name[32];
    switch (shader_type) {
        case GL_VERTEX_SHADER:
            snprintf(shader_name, 32, "vertex");
            break;
        case GL_FRAGMENT_SHADER:
            snprintf(shader_name, 32, "fragment");
            break;
        default:
            ssp_syslog(LOG_ERR, "SSP. Unknown shader type %u", shader_type);
            return 1;
    }
    ssp_syslog(LOG_INFO, "SSP. Shader <%s> was created", shader_name);
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
            ssp_syslog(LOG_ERR, "SSP. Unknown shader type %u", shader_type);
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
    ssp_gl_use_program(ssp_shader.program_id);
}

int ssp_shader_create_program(ssp_shader_info* shaders, int shaders_count)
{
    ssp_shader.program_id = ssp_gl_create_program();
    GLuint shaders_id[shaders_count];

    for (int i = 0; i < shaders_count; i++) {
        shaders_id[i] = ssp_shader_compile(shaders[i].type, shaders[i].data);
        if (shaders_id[i] == 0) {
            ssp_syslog(LOG_ERR, "SSP. Shader creation error: %s", shaders[i].path);
            ssp_shader_destructor();
            return 0;
        }
        ssp_gl_attach_shader(ssp_shader.program_id, shaders_id[i]);
    }

    GLint success;
    ssp_gl_link_program(ssp_shader.program_id);
    ssp_gl_get_programiv(ssp_shader.program_id, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[128];
        ssp_gl_get_program_info_log(ssp_shader.program_id, sizeof(infoLog), NULL, infoLog);
        ssp_syslog(LOG_ERR, "SSP. Gl program link error: %s", infoLog);
        ssp_shader_destructor();
        return 0;
    }

    for (int i = 0; i < shaders_count; i++) {
        ssp_gl_delete_shader(shaders_id[i]);
    }

    return ssp_shader.program_id;
}
