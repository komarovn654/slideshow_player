#pragma once

#define MAX_WINDOW_WIDTH    400
#define MAX_WINDOW_HEIGHT   800

typedef struct ssp_window_t* ssp_window;

int ssp_glfw_init(void);
ssp_window ssp_window_init(int width, int height);
void ssp_window_destruct(ssp_window);
int ssp_player_loop(ssp_window window);