#ifndef render_h
#define render_h

#ifdef __cplusplus
extern "C" {
#endif

#include "ssp_helper.h"
#include "ssp_shader.h"

typedef void (*ssp_render_resize_handler_t)(int width, int height);
typedef int (*ssp_render_init_buffers_t)(void* render);
typedef int (*ssp_render_set_shaders_t)(void* render);
typedef int (*ssp_render_setup_texture_t)(void* render);

typedef struct ssp_render_buffers_t {
    GLuint vao_id; // vertex array object
    GLuint ebo_id; // element buffer object
    GLuint vbo_id; // vertex buffer object
} ssp_render_buffers;

typedef struct ssp_render_t {
    GLfloat vertices[30];
    ssp_shader_info* shaders;
    ssp_render_buffers buffers;
    GLuint texture;

    ssp_render_resize_handler_t resize_handler;
    ssp_render_init_buffers_t init_buffers;
    ssp_render_set_shaders_t set_shaders;
    ssp_render_setup_texture_t setup_texture;
} ssp_render_t;

void ssp_render_set_gl_ctx(void);
int ssp_render_init_buffers(ssp_render_t* render);
int ssp_render_set_shaders(ssp_render_t* render);
int ssp_render_init_glad(void);
void ssp_render_viewport(GLint x_offset, GLint y_offset, GLsizei width, GLsizei height);
int ssp_render_init(ssp_render_resize_handler_t resize_handler);
int ssp_render_redraw(const char* image);

void ssp_render_set_fptr_default(void);
void ssp_render_set_setup_texture(ssp_render_setup_texture_t fptr);
void ssp_render_set_init_buffers(ssp_render_init_buffers_t fptr);
void ssp_render_set_set_shaders(ssp_render_set_shaders_t fptr);

#ifdef __cplusplus
}
#endif

#endif // render_h
