#pragma once

#include <GLFW/glfw3.h>
#include "ssp_helper.h"

#define MAX_SSP_WINDOW_WIDTH    (1920)
#define MAX_SSP_WINDOW_HEIGHT   (1640)

#define SSP_DISPLAY_PLATFORM (_DISPLAY_PLATFORM)

typedef enum {
    SSP_DP_UNKNOWN = 0,
    SSP_DP_WAYLAND = GLFW_PLATFORM_WAYLAND,
    SSP_DP_X11 = GLFW_PLATFORM_X11,
    SSP_DP_COCOA = GLFW_PLATFORM_COCOA,
    SSP_DP_WIN32 = GLFW_PLATFORM_WIN32,
} ssp_display_platform;

int ssp_window_init(int width, int height, double redraw_time, ssp_image_storage* images);
void ssp_window_destruct(void);
int ssp_window_player_loop(void);