#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void ssp_set_glfw_fptr_default(void);

typedef void (*ssp_glfw_window_hint_t)(int, int);
typedef void* (*ssp_glfw_get_proc_address_t)(const char *);
typedef void (*ssp_glfw_get_framebuffer_size_t)(void *, int *, int *);
typedef int (*ssp_glfw_platform_supported_t)(int);
typedef void (*ssp_glfw_init_hint_t)(int, int);
typedef double (*ssp_glfw_get_time_t)(void);
typedef void (*ssp_glfw_set_time_t)(double);
typedef void* (*ssp_glfw_set_error_callback_t)(void*);
typedef const char* (*ssp_glfw_get_version_string_t)(void);
typedef int (*ssp_glfw_init_t)(void);
typedef int (*ssp_glfw_get_platform_t)(void);
typedef void* (*ssp_glfw_create_window_t)(int, int, const char *, void *, void *);
typedef void (*ssp_glfw_make_context_current_t)(void *);
typedef void (*ssp_glfw_terminate_t)(void);
typedef int (*ssp_glfw_window_should_close_t)(void *);
typedef void (*ssp_glfw_swap_buffers_t)(void *);
typedef void (*ssp_glfw_poll_events_t)(void);

void ssp_glfw_set_window_hint(ssp_glfw_window_hint_t);
void ssp_glfw_set_get_proc_address(ssp_glfw_get_proc_address_t);
void ssp_glfw_set_get_framebuffer_size(ssp_glfw_get_framebuffer_size_t);
void ssp_glfw_set_platform_supported(ssp_glfw_platform_supported_t);
void ssp_glfw_set_init_hint(ssp_glfw_init_hint_t);
void ssp_glfw_set_get_time(ssp_glfw_get_time_t);
void ssp_glfw_set_set_time(ssp_glfw_set_time_t);
void ssp_glfw_set_set_error_callback(ssp_glfw_set_error_callback_t);
void ssp_glfw_set_get_version_string(ssp_glfw_get_version_string_t);
void ssp_glfw_set_init(ssp_glfw_init_t);
void ssp_glfw_set_get_platform(ssp_glfw_get_platform_t);
void ssp_glfw_set_create_window(ssp_glfw_create_window_t);
void ssp_glfw_set_make_context_current(ssp_glfw_make_context_current_t);
void ssp_glfw_set_terminate(ssp_glfw_terminate_t);
void ssp_glfw_set_window_should_close(ssp_glfw_window_should_close_t);
void ssp_glfw_set_swap_buffers(ssp_glfw_swap_buffers_t);
void ssp_glfw_set_poll_events(ssp_glfw_poll_events_t);

#ifdef __cplusplus
}
#endif
