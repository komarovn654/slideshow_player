#pragma "once"

#include <wayland-client.h>

void ssp_init_egl(struct wl_display *src_display, struct wl_surface *src_surface);
void ssp_create_window(struct wl_surface *src_surface);

void draw_1();
void draw_2();