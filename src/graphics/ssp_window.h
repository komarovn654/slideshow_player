#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <GLFW/glfw3.h>
#include "ssp_helper.h"

#define MAX_SSP_WINDOW_WIDTH    (1920)
#define MAX_SSP_WINDOW_HEIGHT   (1640)

#ifndef _DISPLAY_PLATFORM
    #define SSP_DISPLAY_PLATFORM (0)
#else
    #define SSP_DISPLAY_PLATFORM (_DISPLAY_PLATFORM)
#endif

typedef enum {
    SSP_DP_UNKNOWN = 0,
    SSP_DP_WAYLAND = GLFW_PLATFORM_WAYLAND,
    SSP_DP_X11 = GLFW_PLATFORM_X11,
    SSP_DP_COCOA = GLFW_PLATFORM_COCOA,
    SSP_DP_WIN32 = GLFW_PLATFORM_WIN32,
} ssp_display_platform;

typedef struct ssp_window_t {
    GLFWwindow *window;
    int width, height, width_pixels, height_pixels;

    double redraw_time;
    ssp_image_storage* images;
    void* head_storage;
    void* current_storage;
} ssp_window_t;

ssp_window_t *ssp_window_get_window(void);
int ssp_window_init(int width, int height, double redraw_time, ssp_image_storage* images);
void ssp_window_destruct(void);
int ssp_window_player_loop(void);

typedef int (*ssp_glfw_init_t)(ssp_display_platform);
typedef void (*ssp_glfw_set_time_t)(double);
typedef void (*ssp_render_set_gl_ctx_t)(void);
typedef GLFWwindow* (*ssp_glfw_create_window_t)(int, int, const char*, GLFWmonitor*, GLFWwindow*);
typedef void (*ssp_glfw_make_context_current_t)(GLFWwindow*);
typedef void (*ssp_window_resize_handler_t)(int, int);
typedef int (*ssp_render_init_t)(ssp_window_resize_handler_t);

void ssp_ws_default_fptr(void);
void ssp_ws_glfw_init(ssp_glfw_init_t f_ptr);
void ssp_ws_glfw_time(ssp_glfw_set_time_t f_ptr);
void ssp_ws_render_set_gl_ctx(ssp_render_set_gl_ctx_t f_ptr);
void ssp_ws_glfw_create_window(ssp_glfw_create_window_t f_ptr);
void ssp_ws_glfw_make_context_current(ssp_glfw_make_context_current_t f_ptr);
void ssp_ws_render_init(ssp_render_init_t f_ptr);
void ssp_ws_window_resize_handler(ssp_window_resize_handler_t f_ptr);

#ifdef __cplusplus
}
#endif
