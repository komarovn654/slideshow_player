#include <stdlib.h>

#include "ssp_glfw.h"
#include "ssp_glfw_mock.h"

struct ssp_glfw_hints_t ssp_glfw_hints;

void ssp_glfw_window_hint_mock_p(int hint, int value) {
    switch (hint)
    {
    case GLFW_CLIENT_API:
        ssp_glfw_hints.client_api = value;
        break;
    case GLFW_CONTEXT_VERSION_MAJOR:
        ssp_glfw_hints.context_version_major = value;
        break;
    case GLFW_CONTEXT_VERSION_MINOR:
        ssp_glfw_hints.context_version_minor = value;
        break;
    case GLFW_OPENGL_FORWARD_COMPAT:
        ssp_glfw_hints.opengl_forward_compact = value;
        break;
    case GLFW_OPENGL_PROFILE:
        ssp_glfw_hints.opengl_profile = value;
        break;
    default:
        break;
    }
    return;
}

void* ssp_glfw_get_proc_address_mock_p(const char *procname) {
    return (void*)1;
}

void* ssp_glfw_get_proc_address_mock_f(const char *procname) {
    return NULL;
}

void ssp_glfw_get_framebuffer_size_mock_p(void *handle, int *width, int *height) {
    return;
}

int ssp_glfw_platform_supported_mock_p(int platformID) {
    return 1;
}

int ssp_glfw_platform_supported_mock_f(int platformID) {
    return 0;
}

void ssp_glfw_init_hint_mock_p(int hint, int value) {
    return;
}

double ssp_glfw_get_time_mock_p(void) {
    return 1.0;
}

double ssp_glfw_get_time_mock_f(void) {
    return 0.0;
}

void ssp_glfw_set_time_mock_p(double time) {
    return;
}

void* ssp_glfw_set_error_callback_mock_p(void *cbfun) {
    return (void*)1;
}

void* ssp_glfw_set_error_callback_mock_f(void *cbfun) {
    return NULL;
}

const char* ssp_glfw_get_version_string_mock_p(void) {
    return "string";
}

const char* ssp_glfw_get_version_string_mock_f(void) {
    return NULL;
}

int ssp_glfw_init_mock_p(void) {
    return 1;
}

int ssp_glfw_init_mock_f(void) {
    return 0;
}

int ssp_glfw_get_platform_mock_p(void) {
    return 1;
}

int ssp_glfw_get_platform_mock_f(void) {
    return 0;
}

void* ssp_glfw_create_window_mock_p(int width, int height, const char *title, void *monitor, void *share) {
    return (void*)1;
}

void* ssp_glfw_create_window_mock_f(int width, int height, const char *title, void *monitor, void *share) {
    return NULL;
}

void ssp_glfw_make_context_current_mock_p(void *handle) {
    return;
}

void ssp_glfw_terminate_mock_p(void) {
    return;
}

int ssp_glfw_window_should_close_mock_p(void *handle) {
    return 1;
}

int ssp_glfw_window_should_close_mock_f(void *handle) {
    return 0;
}

void ssp_glfw_swap_buffers_mock_p(void *handle) {
    return;
}

void ssp_glfw_poll_events_mock_p(void) {
    return;
}

