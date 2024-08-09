#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "GLFW/glfw3.h"

void ssp_glfw_window_hint(int hint, int value);
void* ssp_glfw_get_proc_address(const char *procname);
void ssp_glfw_get_framebuffer_size(void *handle, int *width, int *height);
int ssp_glfw_platform_supported(int platformID);
void ssp_glfw_init_hint(int hint, int value);
double ssp_glfw_get_time(void);
void ssp_glfw_set_time(double time);
void* ssp_glfw_set_error_callback(void *cbfun);
const char* ssp_glfw_get_version_string(void);
int ssp_glfw_init(void);
int ssp_glfw_get_platform(void);
void* ssp_glfw_create_window(int width, int height, const char *title, void *monitor, void *share);
void ssp_glfw_make_context_current(void *handle);
void ssp_glfw_terminate(void);
int ssp_glfw_window_should_close(void *handle);
void ssp_glfw_swap_buffers(void *handle);
void ssp_glfw_poll_events(void);

#ifdef __cplusplus
}
#endif
