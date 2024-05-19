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
    shader_meta shaders[2];
    ssp_render_buffers buffers;
    GLuint texture;
} ssp_render;

int ssp_render_init(void);
void ssp_render_set_gl_ctx(void);
int ssp_render_redraw(const char* image);
int test_redraw(const char* image);
