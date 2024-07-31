#include "ssp_gl.h"

typedef struct ssp_gl_t {
    ssp_glfw_window_hint_t window_hint;
    ssp_glfw_get_proc_address_t get_proc_address;
    ssp_glfw_get_framebuffer_size_t get_framebuffer_size;
    ssp_glfw_platform_supported_t platform_supported;
    ssp_glfw_init_hint_t init_hint;
    ssp_glfw_get_time_t get_time;
    ssp_glfw_set_error_callback_t set_error_callback;
    ssp_glfw_get_version_string_t get_version_string;
    ssp_glfw_init_t init;
    ssp_glfw_get_platform_t get_platform;
    ssp_glfw_create_window_t create_window;
    ssp_glfw_make_context_current_t make_context_current;
} ssp_gl_t;