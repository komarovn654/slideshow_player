#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ssp_glfw_hints_t {
    int client_api;
    int context_version_major;
    int context_version_minor;
    int opengl_forward_compact;
    int opengl_profile;
} ssp_glfw_hints_t;

void ssp_glfw_window_hint_mock_p(int hint, int value);
void* ssp_glfw_get_proc_address_mock_p(const char *procname);
void* ssp_glfw_get_proc_address_mock_f(const char *procname);
void ssp_glfw_get_framebuffer_size_mock_p(void *handle, int *width, int *height);
int ssp_glfw_platform_supported_mock_p(int platformID);
int ssp_glfw_platform_supported_mock_f(int platformID);
void ssp_glfw_init_hint_mock_p(int hint, int value);
double ssp_glfw_get_time_mock_p(void);
double ssp_glfw_get_time_mock_f(void);
void ssp_glfw_set_time_mock_p(double time);
void* ssp_glfw_set_error_callback_mock_p(void *cbfun);
void* ssp_glfw_set_error_callback_mock_f(void *cbfun);
const char* ssp_glfw_get_version_string_mock_p(void);
const char* ssp_glfw_get_version_string_mock_f(void);
int ssp_glfw_init_mock_p(void);
int ssp_glfw_init_mock_f(void);
int ssp_glfw_get_platform_mock_p(void);
int ssp_glfw_get_platform_mock_f(void);
void* ssp_glfw_create_window_mock_p(int width, int height, const char *title, void *monitor, void *share);
void* ssp_glfw_create_window_mock_f(int width, int height, const char *title, void *monitor, void *share);
void ssp_glfw_make_context_current_mock_p(void *handle);
void ssp_glfw_terminate_mock_p(void);
int ssp_glfw_window_should_close_mock_p(void *handle);
int ssp_glfw_window_should_close_mock_f(void *handle);
void ssp_glfw_swap_buffers_mock_p(void *handle);
void ssp_glfw_poll_events_mock_p(void);

#ifdef __cplusplus
}
#endif
