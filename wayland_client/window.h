#pragma once

#include "wayland-client.h"
#include "xdg-shell-client-protocol.h"
#include <sys/types.h>

typedef struct globals
{
    struct wl_display *display;
    struct wl_registry *registry;
    struct wl_compositor *compositor;
    struct wl_shm *shm;
    struct xdg_wm_base *xdg_base;
} globals;

typedef struct window
{
    globals globals;

    struct wl_surface *surface;
    struct xdg_surface *xdg_surface;
    struct xdg_toplevel *xdg_toplevel;
    struct wl_callback *callback;
    clockid_t clk_id;
} window;

int create_display(void);
window *get_client_window(void);
