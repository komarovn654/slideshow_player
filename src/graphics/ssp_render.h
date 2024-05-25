#pragma once

#include <GLFW/glfw3.h>
#include "ssp_helper.h"
#include "ssp_shader.h"

#define SSP_RENDER_TEXTURE_VERTICES {  \
    -1.0f,  1.0f, 0.0f,     0.0f, 1.0f,\
    -1.0f, -1.0f, 0.0f,     0.0f, 0.0f,\
     1.0f, -1.0f, 0.0f,     1.0f, 0.0f,\
     1.0f, -1.0f, 0.0f,     1.0f, 0.0f,\
     1.0f,  1.0f, 0.0f,     1.0f, 1.0f,\
    -1.0f,  1.0f, 0.0f,     0.0f, 1.0f,\
}

typedef struct ssp_render_buffers_t {
    GLuint vao_id; // vertex array object
    GLuint ebo_id; // element buffer object
    GLuint vbo_id; // vertex buffer object
} ssp_render_buffers;

typedef struct ssp_render_t {
    GLfloat vertices[30];
    ssp_shader_info shaders[2];
    ssp_render_buffers buffers;
    GLuint texture;
} ssp_render;

void ssp_render_set_gl_ctx(void);
void ssp_render_init_buffers(ssp_render* render);
void ssp_render_set_shaders(ssp_render* render);

int ssp_render_init(void);
int ssp_render_redraw(const char* image);
