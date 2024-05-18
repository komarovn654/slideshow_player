#pragma once

#include <GLFW/glfw3.h>
#include "ssp_helper.h"

typedef struct ssp_render_buffers_t {
    GLuint vao_id; // vertex array object
    GLuint ebo_id; // element buffer object
    GLuint vbo_id; // vertex buffer object
} ssp_render_buffers;

int ssp_render_init(void);
int ssp_render_redraw(const char* image);
int test_redraw(const char* image);
