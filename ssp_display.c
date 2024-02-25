#include <assert.h>
#include <wayland-client.h>
#include <wayland-egl.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <logger.h>
#include <string.h>

#include "ssp_egl.h"

static struct t_display {
    // wayland's globals
    struct wl_display *display;
    struct wl_registry *registry;
    struct wl_compositor *compositor;
    struct wl_surface *surface;
    struct wl_shell *shell;
    struct wl_shell_surface *shell_surface;

    // egl
    struct wl_egl_window *egl_window;
    EGLDisplay egl_display;
    EGLConfig egl_conf;
    EGLSurface egl_surface;
    EGLContext egl_context;
} display;

static void registry_handle_global(void *data, struct wl_registry *registry, uint32_t name, 
    const char *interface, uint32_t version)
{
    struct t_display *d = data;

    if (strcmp(interface, "wl_compositor") == 0) {
		d->compositor = wl_registry_bind(registry, name, &wl_compositor_interface, 1);
    }
    else if (strcmp(interface, "wl_shell") == 0) {
	    d->shell = wl_registry_bind(registry, name, &wl_shell_interface, 1);
    }
}

static void registry_handle_global_remove(void *data, struct wl_registry *registry, uint32_t name)
{
}

static const struct wl_registry_listener registry_listener = {
	registry_handle_global,
	registry_handle_global_remove
};

int ssp_init_display(void)
{
    /* get server references */
	display.display = wl_display_connect(NULL);
	assert(display.display);
    log_debug("display connected");

	display.registry = wl_display_get_registry(display.display);
    assert(display.registry);
	wl_registry_add_listener(display.registry, &registry_listener, &display);
    log_debug("registry getted");
    
	if (wl_display_roundtrip(display.display) == -1) {
        log_error("roundtrip error");
    }
    // TODO: check compositor, shell here

    /* setup surfaces */
    display.surface = wl_compositor_create_surface(display.compositor);
    if (display.surface == NULL) {
	    log_error("surface error");
    }
	log_debug("surface created");

    display.shell_surface = wl_shell_get_shell_surface(display.shell, display.surface);
    wl_shell_surface_set_toplevel(display.shell_surface);
    log_debug("shell-surface created");
    // TODO: check shell_surface here

    ssp_init_egl(display.display, display.surface);
    log_debug("egl inited");

    ssp_create_window(display.surface);
    log_debug("window inited");
    
    return 0;
}

