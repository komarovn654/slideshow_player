#ifndef render_h
#define render_h

#ifdef __cplusplus
extern "C" {
#endif

#include "ssp_helper.h"
#include "ssp_shader.h"

typedef struct ssp_render_t* ssp_render;

void ssp_render_set_gl_ctx(void);
int ssp_render_init_buffers(ssp_render render);
int ssp_render_set_shaders(ssp_render render);
int ssp_render_init_glad(void);
void ssp_render_viewport(GLint x_offset, GLint y_offset, GLsizei width, GLsizei height);
// int ssp_render_init(ssp_render_resize_handler_t resize_handler);
int ssp_render_redraw(const char* image);

#ifdef __cplusplus
}
#endif

#endif // render_h
