#include "ssp_window.h"
#include "ssp_render.h"
#include "ssp_graphics_mock.h"

int ssp_glfw_init_mock_error(ssp_display_platform platform) { return 1; }
int ssp_glfw_init_mock_pass(ssp_display_platform platform) { return 0; }

void ssp_glfw_set_time_mock(double time) { return; }

void ssp_render_set_gl_ctx_mock(void) { return; }

GLFWwindow* ssp_glfw_create_window_error(int p1, int p2, const char* p3, GLFWmonitor* p4, GLFWwindow* p5)
{
    return NULL;
}
GLFWwindow* ssp_glfw_create_window_pass(int p1, int p2, const char* p3, GLFWmonitor* p4, GLFWwindow* p5)
{
    static GLFWwindow* window;
    return window;
}

int ssp_glad_load_glloader_mock_error(GLADloadproc)
{
    return 0;
}

int ssp_glad_load_glloader_mock_pass(GLADloadproc)
{
    return 1;
}

// typedef void (*ssp_glfw_make_context_current_t)(GLFWwindow*);
// typedef void (*ssp_window_resize_handler_t)(int, int);
// typedef int (*ssp_render_init_t)(ssp_window_resize_handler_t);