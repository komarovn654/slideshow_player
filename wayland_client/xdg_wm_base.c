#include <stdio.h>
#include <sys/mman.h>

#include "xdg-shell-client-protocol.h"
#include "xdg_wm_base.h"

#include "window.h"
#include "shm.h"
#include "image.h"

static void wl_buffer_release(void *data, struct wl_buffer *wl_buffer)
{
    /* Sent by the compositor when it's no longer using this buffer */
    wl_buffer_destroy(wl_buffer);
}

static const struct wl_buffer_listener wl_buffer_listener = {
    .release = wl_buffer_release,
};

static struct wl_buffer *draw_frame(struct wl_shm *shm, char* file_name)
{
    const int width = 720, height = 1280;
    int stride = width * 4;
    int size = stride * height;

    int fd = allocate_shm_file(size);
    if (fd == -1) {
        return NULL;
    }

    uint32_t *data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) {
        close(fd);
        return NULL;
    }

    struct wl_shm_pool *pool = wl_shm_create_pool(shm, fd, size);
    struct wl_buffer *buffer = wl_shm_pool_create_buffer(pool, 0, width, height, stride, WL_SHM_FORMAT_XRGB8888);
    wl_shm_pool_destroy(pool);
    close(fd);

    pixman_image_t* img = load_image(file_name);
    uint32_t* d = pixman_image_get_data(img);
    for (int i = 0; i < height * width; ++i) {
        data[i] = d[i];
    }

    munmap(data, size);
    wl_buffer_add_listener(buffer, &wl_buffer_listener, NULL);
    return buffer;
}

static void xdg_wm_base_ping(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial)
{
    xdg_wm_base_pong(xdg_wm_base, serial);
}

static const struct xdg_wm_base_listener wm_base_listener = {
    .ping = xdg_wm_base_ping,
};

static void xdg_surface_configure(void *data, struct xdg_surface *xdg_surface, uint32_t serial)
{
    window *client = data;
    xdg_surface_ack_configure(xdg_surface, serial);
/*
    struct wl_buffer *buffer = draw_frame(client->globals.shm);
    wl_surface_attach(client->surface, buffer, 0, 0);
    wl_surface_commit(client->surface);
*/
}

static const struct xdg_surface_listener surface_listener = {
    .configure = xdg_surface_configure,
};

const struct xdg_wm_base_listener *get_xdg_wm_base_listener(void)
{
    return &wm_base_listener;
}

void init_xdg_surface(window *client_window)
{
    client_window->xdg_surface = xdg_wm_base_get_xdg_surface(client_window->globals.xdg_base, client_window->surface);
    xdg_surface_add_listener(client_window->xdg_surface, &surface_listener, client_window);
    client_window->xdg_toplevel = xdg_surface_get_toplevel(client_window->xdg_surface);
    xdg_toplevel_set_title(client_window->xdg_toplevel, "Slide Show Player");
    
    return;
}

void draw_picture(char *file_name)
{
    window *client = get_client_window();

    struct wl_buffer *buffer = draw_frame(client->globals.shm, file_name);
    wl_surface_attach(client->surface, buffer, 0, 0);
    wl_surface_commit(client->surface);
}
