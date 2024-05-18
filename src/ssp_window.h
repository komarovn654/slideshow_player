#pragma once

#include <GLFW/glfw3.h>

#define MAX_WINDOW_WIDTH    960
#define MAX_WINDOW_HEIGHT   1280

typedef enum {
    SSP_DP_UNKNOWN = 0,
    SSP_DP_WAYLAND = GLFW_PLATFORM_WAYLAND,
    SSP_DP_X11 = GLFW_PLATFORM_X11,
    SSP_DP_COCOA = GLFW_PLATFORM_COCOA,
} ssp_display_platform;

int ssp_window_init(int width, int height, double redraw_time, ssp_image_storage* images);
void ssp_window_destruct(void);
int ssp_window_player_loop(void);