#pragma once

#define MAX_WINDOW_WIDTH    400
#define MAX_WINDOW_HEIGHT   800

typedef struct ssp_window_t* ssp_window;

ssp_window ssp_window_init(int width, int height);