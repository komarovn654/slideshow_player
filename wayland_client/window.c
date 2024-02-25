#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

#include "wayland-client.h"
#include "window.h"

#include "xdg-shell-client-protocol.h"
#include "xdg_wm_base.h"

static window client_window = { 0 };

window *get_client_window(void)
{
    return &client_window;
}

static int validate_globals(globals *gl)
{
    if (gl->compositor == NULL) {
        fprintf(stderr, "ERROR: can't find global: wl_compositor\n");
        return EXIT_FAILURE;
    } else if (gl->xdg_base == NULL) {
        fprintf(stderr, "ERROR: can't find global: xdg_wm_base\n");
        return EXIT_FAILURE;
    }
    else if (gl->shm == NULL) {
        fprintf(stderr, "ERROR: can't find global: wl_shm\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

static void global_registry(void *data, struct wl_registry *wl_registry, uint32_t name, const char *interface, uint32_t version)
{
    struct window *client = data;

    if (strcmp(interface, wl_compositor_interface.name) == 0) {
        client->globals.compositor = wl_registry_bind(wl_registry, name, &wl_compositor_interface, version);
    } else if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
        client->globals.xdg_base = wl_registry_bind(wl_registry, name, &xdg_wm_base_interface, version);
        xdg_wm_base_add_listener(client->globals.xdg_base, get_xdg_wm_base_listener(), data);
    } else if (strcmp(interface, wl_shm_interface.name) == 0) {
        client->globals.shm = wl_registry_bind(wl_registry, name, &wl_shm_interface, version);
    }
}

static void global_remove_registry(void *data, struct wl_registry *wl_registry, uint32_t name)
{

}

static const struct wl_registry_listener registry_listener = {
    .global = global_registry,
    .global_remove = global_remove_registry,
};

static const struct wl_callback_listener frame_listener;

static void redraw(void *data, struct wl_callback *callback, uint32_t time)
{
    fprintf(stderr, "Redrawing\n");
    wl_callback_destroy(client_window.callback);

    client_window.callback = wl_surface_frame(client_window.surface);
    wl_callback_add_listener(client_window.callback, &frame_listener, NULL);
    
    wl_surface_commit(client_window.surface); 
    fprintf(stderr, "Redrawing exit\n");
}

static const struct wl_callback_listener frame_listener = {
    redraw
};

int create_display()
{
    client_window.globals.display = wl_display_connect(NULL);
    if (client_window.globals.display == NULL) {
        fprintf(stderr, "ERROR: can't connect to display\n");
        return EXIT_FAILURE;
    }

    client_window.globals.registry = wl_display_get_registry(client_window.globals.display);
    wl_registry_add_listener(client_window.globals.registry, &registry_listener, &client_window);

    wl_display_roundtrip(client_window.globals.display);
    if (validate_globals(&client_window.globals) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    client_window.surface = wl_compositor_create_surface(client_window.globals.compositor);
    if (client_window.surface == NULL) {
        fprintf(stderr, "ERROR: can't create surface out of compositor\n");
        return EXIT_FAILURE;
    }

    init_xdg_surface(&client_window);
    wl_surface_commit(client_window.surface);

    while (wl_display_dispatch(client_window.globals.display)) {
        /* This space deliberately left blank */
    }

   return EXIT_SUCCESS;
}
