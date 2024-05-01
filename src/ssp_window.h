#pragma once

#include <GLFW/glfw3.h>
#include "ssp_list.h"

#define MAX_WINDOW_WIDTH    960
#define MAX_WINDOW_HEIGHT   1280

typedef struct ssp_window_t* ssp_window;

typedef enum {
    DP_UNKNOWN = 0,
    DP_WAYLAND = GLFW_PLATFORM_WAYLAND,
    DP_X11 = GLFW_PLATFORM_X11,
    DP_COCOA = GLFW_PLATFORM_COCOA,
} ssp_display_platform;

int ssp_glfw_init(ssp_display_platform platform);
ssp_window ssp_window_init(int width, int height, double redraw_time, ssp_list images);
void ssp_window_destruct(ssp_window);
int ssp_player_loop(ssp_window window);