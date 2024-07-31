#ifndef graphics_mock_h
#define graphics_mock_h

#include "ssp_window.h"
#include "ssp_render.h"

#ifdef __cplusplus
extern "C" {
#endif

int ssp_glfw_init_mock_error(ssp_display_platform platform);
int ssp_glfw_init_mock_pass(ssp_display_platform platform);
void ssp_glfw_set_time_mock(double time);
void ssp_render_set_gl_ctx_mock(void);
GLFWwindow* ssp_glfw_create_window_error(int, int, const char*, GLFWmonitor*, GLFWwindow*);
GLFWwindow* ssp_glfw_create_window_pass(int, int, const char*, GLFWmonitor*, GLFWwindow*);

int ssp_glad_load_glloader_mock_error(GLADloadproc);
int ssp_glad_load_glloader_mock_pass(GLADloadproc);

#ifdef __cplusplus
}
#endif

#endif
