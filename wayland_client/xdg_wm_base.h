#pragma once

#include "xdg-shell-client-protocol.h"
#include "window.h"

void init_xdg_surface(window *client_window);
const struct xdg_wm_base_listener *get_xdg_wm_base_listener(void);
void draw_picture(char *file_name);