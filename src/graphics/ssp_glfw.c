#include "ssp_glfw.h"
#include "ssp_glfw_set.h"

typedef struct ssp_glfw_t {
    ssp_glfw_window_hint_t window_hint;
    ssp_glfw_get_proc_address_t get_proc_address;
    ssp_glfw_get_framebuffer_size_t get_framebuffer_size;
    ssp_glfw_platform_supported_t platform_supported;
    ssp_glfw_init_hint_t init_hint;
    ssp_glfw_get_time_t get_time;
    ssp_glfw_set_time_t set_time;
    ssp_glfw_set_error_callback_t set_error_callback;
    ssp_glfw_get_version_string_t get_version_string;
    ssp_glfw_init_t init;
    ssp_glfw_get_platform_t get_platform;
    ssp_glfw_create_window_t create_window;
    ssp_glfw_make_context_current_t make_context_current;
    ssp_glfw_terminate_t terminate;
    ssp_glfw_window_should_close_t window_should_close;
    ssp_glfw_swap_buffers_t swap_buffers;
    ssp_glfw_poll_events_t poll_events;
} ssp_glfw_t;

static ssp_glfw_t ssp_glfw = {
    .window_hint = glfwWindowHint,
    .get_proc_address = (ssp_glfw_get_proc_address_t)glfwGetProcAddress,
    .get_framebuffer_size = (ssp_glfw_get_framebuffer_size_t)glfwGetFramebufferSize,
    .platform_supported = glfwPlatformSupported,
    .init_hint = glfwInitHint,
    .get_time = glfwGetTime,
    .set_time = glfwSetTime,
    .set_error_callback = (ssp_glfw_set_error_callback_t)glfwSetErrorCallback,
    .get_version_string = glfwGetVersionString,
    .init = glfwInit,
    .get_platform = glfwGetPlatform,
    .create_window = (ssp_glfw_create_window_t)glfwCreateWindow,
    .make_context_current = (ssp_glfw_make_context_current_t)glfwMakeContextCurrent,
    .terminate = glfwTerminate,
    .window_should_close = (ssp_glfw_window_should_close_t)glfwWindowShouldClose,
    .swap_buffers = (ssp_glfw_swap_buffers_t)glfwSwapBuffers,
    .poll_events = glfwPollEvents,
};

void ssp_set_glfw_fptr_default(void)
{
    ssp_glfw.window_hint = glfwWindowHint;
    ssp_glfw.get_proc_address = (ssp_glfw_get_proc_address_t)glfwGetProcAddress;
    ssp_glfw.get_framebuffer_size = (ssp_glfw_get_framebuffer_size_t)glfwGetFramebufferSize;
    ssp_glfw.platform_supported = glfwPlatformSupported;
    ssp_glfw.init_hint = glfwInitHint;
    ssp_glfw.get_time = glfwGetTime;
    ssp_glfw.set_time = glfwSetTime;
    ssp_glfw.set_error_callback = (ssp_glfw_set_error_callback_t)glfwSetErrorCallback;
    ssp_glfw.get_version_string = glfwGetVersionString;
    ssp_glfw.init = glfwInit;
    ssp_glfw.get_platform = glfwGetPlatform;
    ssp_glfw.create_window = (ssp_glfw_create_window_t)glfwCreateWindow;
    ssp_glfw.make_context_current = (ssp_glfw_make_context_current_t)glfwMakeContextCurrent;
    ssp_glfw.terminate = glfwTerminate;
    ssp_glfw.window_should_close = (ssp_glfw_window_should_close_t)glfwWindowShouldClose;
    ssp_glfw.swap_buffers = (ssp_glfw_swap_buffers_t)glfwSwapBuffers;
    ssp_glfw.poll_events = glfwPollEvents;
}
// TODO: Check cast types
void ssp_glfw_window_hint(int hint, int value)
{
    return ssp_glfw.window_hint(hint, value);
}

void* ssp_glfw_get_proc_address(const char *procname)
{
    return ssp_glfw.get_proc_address(procname);
}

void ssp_glfw_get_framebuffer_size(void *window, int *width, int *height)
{
    return ssp_glfw.get_framebuffer_size(window, width, height);
}

int ssp_glfw_platform_supported(int platform)
{
    return ssp_glfw.platform_supported(platform);
}

void ssp_glfw_init_hint(int hint, int value)
{
    return ssp_glfw.init_hint(hint, value);
}

double ssp_glfw_get_time(void)
{
    return ssp_glfw.get_time();
}

void ssp_glfw_set_time(double time)
{
    return ssp_glfw.set_time(time);
}

void* ssp_glfw_set_error_callback(void* callback)
{
    return ssp_glfw.set_error_callback(callback);
}

const char* ssp_glfw_get_version_string(void)
{
    return ssp_glfw.get_version_string();
}

int ssp_glfw_init(void)
{
    return ssp_glfw.init();
}

int ssp_glfw_get_platform(void)
{
    return ssp_glfw.get_platform();
}

void* ssp_glfw_create_window(int width, int height, const char *title, void *monitor, void *share)
{
    return ssp_glfw.create_window(width, height, title, monitor, share);
}

void ssp_glfw_make_context_current(void *window)
{
    return ssp_glfw.make_context_current(window);
}

void ssp_glfw_terminate(void)
{
    return ssp_glfw.terminate();
}

int ssp_glfw_window_should_close(void* window)
{
    return ssp_glfw.window_should_close(window);
}

void ssp_glfw_swap_buffers(void* window)
{
    return ssp_glfw.swap_buffers(window);
}

void ssp_glfw_poll_events(void)
{
    return ssp_glfw.poll_events();
}


void ssp_glfw_set_window_hint(ssp_glfw_window_hint_t fptr)
{
    ssp_glfw.window_hint = fptr;
}

void ssp_glfw_set_get_proc_address(ssp_glfw_get_proc_address_t fptr)
{
    ssp_glfw.get_proc_address = fptr;
}

void ssp_glfw_set_get_framebuffer_size(ssp_glfw_get_framebuffer_size_t fptr)
{
    ssp_glfw.get_framebuffer_size = fptr;
}

void ssp_glfw_set_platform_supported(ssp_glfw_platform_supported_t fptr)
{
    ssp_glfw.platform_supported = fptr;
}

void ssp_glfw_set_init_hint(ssp_glfw_init_hint_t fptr)
{
    ssp_glfw.init_hint = fptr;
}

void ssp_glfw_set_get_time(ssp_glfw_get_time_t fptr)
{
    ssp_glfw.get_time = fptr;
}

void ssp_glfw_set_set_time(ssp_glfw_set_time_t fptr)
{
    ssp_glfw.set_time = fptr;
}

void ssp_glfw_set_set_error_callback(ssp_glfw_set_error_callback_t fptr)
{
    ssp_glfw.set_error_callback = fptr;
}

void ssp_glfw_set_get_version_string(ssp_glfw_get_version_string_t fptr)
{
    ssp_glfw.get_version_string = fptr;
}

void ssp_glfw_set_init(ssp_glfw_init_t fptr)
{
    ssp_glfw.init = fptr;
}

void ssp_glfw_set_get_platform(ssp_glfw_get_platform_t fptr)
{
    ssp_glfw.get_platform = fptr;
}

void ssp_glfw_set_create_window(ssp_glfw_create_window_t fptr)
{
    ssp_glfw.create_window = fptr;
}

void ssp_glfw_set_make_context_current(ssp_glfw_make_context_current_t fptr)
{
    ssp_glfw.make_context_current = fptr;
}

void ssp_glfw_set_terminate(ssp_glfw_terminate_t fptr)
{
    ssp_glfw.terminate = fptr;
}

void ssp_glfw_set_window_should_close(ssp_glfw_window_should_close_t fptr)
{
    ssp_glfw.window_should_close = fptr;
}

void ssp_glfw_set_swap_buffers(ssp_glfw_swap_buffers_t fptr)
{
    ssp_glfw.swap_buffers = fptr;
}

void ssp_glfw_set_poll_events(ssp_glfw_poll_events_t fptr)
{
    ssp_glfw.poll_events = fptr;
}
