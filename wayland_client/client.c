#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "shm.h"
#include <sys/mman.h>
#include <wayland-client.h>
#include <wayland-egl.h>
#include "EGL/egl.h"
#include <EGL/eglext.h>
#include "xdg-shell-client-protocol.h"
#include "image.h"
#include "client.h"
#include "time.h"
#include "presentation-protocol.h"

typedef struct feedback
{

} feedback;

static void wp_discarded(void *data, struct wp_presentation_feedback *wp_presentation_feedback)
{
    printf("wp_discarded\n");
}

static void wp_presented(void *data, struct wp_presentation_feedback *wp_presentation_feedback, uint32_t tv_sec_hi, uint32_t tv_sec_lo, uint32_t tv_nsec, uint32_t refresh, uint32_t seq_hi, uint32_t seq_lo, uint32_t flags)
{
	printf("wp_presented\n");
}

static void wp_sync_output(void *data, struct wp_presentation_feedback *wp_presentation_feedback, struct wl_output *output)
{
	printf("wp_sync_output\n");
}

static const struct wp_presentation_feedback_listener feedback_listener = {
    .discarded = wp_discarded,
    .presented = wp_presented,
    .sync_output = wp_sync_output,
};

static void creat_feedback(struct window *window, uint32_t frame_stamp)
{
	static unsigned seq;
	struct wp_presentation *pres = window->display->presentation;
	struct feedback *feedback;

	seq++;

	if (!pres)
		return;

	feedback = zalloc(sizeof *feedback);
	if (!feedback)
		return;

	feedback->window = window;
	feedback->feedback = wp_presentation_feedback(pres, window->surface);
	wp_presentation_feedback_add_listener(feedback->feedback,
					      &feedback_listener, feedback);

	feedback->frame_no = seq;

	clock_gettime(window->display->clk_id, &feedback->commit);
	feedback->frame_stamp = frame_stamp;
	feedback->target = feedback->commit;

	wl_list_insert(&window->feedback_list, &feedback->link);
}

static void wp_clock_id(void *data, struct wp_presentation *wp_presentation, uint32_t clk_id)
{
	struct client_state *cs = data;

	cs->clk_id = clk_id;

    printf("%u\n", cs->clk_id);
}

static const struct wp_presentation_listener presentation_listener = {
    .clock_id = wp_clock_id,
};

static void wl_buffer_release(void *data, struct wl_buffer *wl_buffer)
{
    /* Sent by the compositor when it's no longer using this buffer */
    wl_buffer_destroy(wl_buffer);
}

static const struct wl_buffer_listener wl_buffer_listener = {
    .release = wl_buffer_release,
};

static struct wl_buffer *draw_frame(struct wl_shm *shm)
{
    const int width = 720, height = 1280;
    int stride = width * 4;
    int size = stride * height;

    int fd = allocate_shm_file(size);
    if (fd == -1) {
        return NULL;
    }

    uint32_t *data = mmap(NULL, size,
            PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) {
        close(fd);
        return NULL;
    }

    struct wl_shm_pool *pool = wl_shm_create_pool(shm, fd, size);
    struct wl_buffer *buffer = wl_shm_pool_create_buffer(pool, 0,
            width, height, stride, WL_SHM_FORMAT_XRGB8888);
    wl_shm_pool_destroy(pool);
    close(fd);

    char* jpg = "/home/nikolay/dev/pixman_example/photo_2021-08-13_16-05-04.jpg";
    pixman_image_t* img = load_image(jpg);
    uint32_t* d = pixman_image_get_data(img);
    for (int i = 0; i < height * width; ++i) {
        data[i] = d[i];
    }

    munmap(data, size);
    wl_buffer_add_listener(buffer, &wl_buffer_listener, NULL);
    return buffer;
}

static struct wl_buffer *draw_frame2(struct wl_shm *shm)
{
    printf("call\n");
    const int width = 720, height = 1280;
    int stride = width * 4;
    int size = stride * height;

    int fd = allocate_shm_file(size);
    if (fd == -1) {
        return NULL;
    }

    uint32_t *data = mmap(NULL, size,
            PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) {
        close(fd);
        return NULL;
    }

    struct wl_shm_pool *pool = wl_shm_create_pool(shm, fd, size);
    struct wl_buffer *buffer = wl_shm_pool_create_buffer(pool, 0,
            width, height, stride, WL_SHM_FORMAT_XRGB8888);
    wl_shm_pool_destroy(pool);
    close(fd);

    char* jpg = "/home/nikolay/Pictures/photo_2023-08-20_15-13-23.jpg";
    pixman_image_t* img = load_image(jpg);
    uint32_t* d = pixman_image_get_data(img);
    for (int i = 0; i < height * width; ++i) {
        data[i] = d[i];
    }

    munmap(data, size);
    wl_buffer_add_listener(buffer, &wl_buffer_listener, NULL);
    return buffer;
}

static void xdg_surface_configure(void *data,
        struct xdg_surface *xdg_surface, uint32_t serial)
{
    struct client_state *state = data;
    xdg_surface_ack_configure(xdg_surface, serial);

    struct wl_buffer *buffer = draw_frame(state->wl_shm);
    wl_surface_attach(state->wl_surface, buffer, 0, 0);
    wl_surface_commit(state->wl_surface);
}

static const struct xdg_surface_listener xdg_surface_listener = {
    .configure = xdg_surface_configure,
};

static void xdg_wm_base_ping(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial)
{
    xdg_wm_base_pong(xdg_wm_base, serial);
}

static const struct xdg_wm_base_listener xdg_wm_base_listener = {
    .ping = xdg_wm_base_ping,
};

static void registry_global(void *data, struct wl_registry *wl_registry,
        uint32_t name, const char *interface, uint32_t version)
{
    struct client_state *state = data;
    printf("gloabal: %s\n", interface);

    if (strcmp(interface, wl_shm_interface.name) == 0) {
        state->wl_shm = wl_registry_bind(
                wl_registry, name, &wl_shm_interface, 1);
    } else if (strcmp(interface, wl_compositor_interface.name) == 0) {
        state->wl_compositor = wl_registry_bind(
                wl_registry, name, &wl_compositor_interface, 4);
    } else if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
        state->xdg_wm_base = wl_registry_bind(
                wl_registry, name, &xdg_wm_base_interface, 1);
        xdg_wm_base_add_listener(state->xdg_wm_base,
                &xdg_wm_base_listener, state);
    } else if (strcmp(interface, wp_presentation_interface.name) == 0) {
        state->wp_presentation = wl_registry_bind(
                wl_registry, name, &wp_presentation_interface, 1);
        wp_presentation_add_listener(state->wp_presentation, 
                &presentation_listener, state);

    }   
}

static void registry_global_remove(void *data,
        struct wl_registry *wl_registry, uint32_t name)
{
    /* This space deliberately left blank */
}

static const struct wl_registry_listener wl_registry_listener = {
    .global = registry_global,
    .global_remove = registry_global_remove,
};

int main(int argc, char *argv[])
{
    struct client_state state = { 0 };
    state.wl_display = wl_display_connect(NULL);
    state.wl_registry = wl_display_get_registry(state.wl_display);
    wl_registry_add_listener(state.wl_registry, &wl_registry_listener, &state);
    wl_display_roundtrip(state.wl_display);

    state.wl_surface = wl_compositor_create_surface(state.wl_compositor);
    state.xdg_surface = xdg_wm_base_get_xdg_surface(state.xdg_wm_base, state.wl_surface);
    xdg_surface_add_listener(state.xdg_surface, &xdg_surface_listener, &state);
    state.xdg_toplevel = xdg_surface_get_toplevel(state.xdg_surface);
    xdg_toplevel_set_title(state.xdg_toplevel, "Example client");
    wl_surface_commit(state.wl_surface);

    while (wl_display_dispatch(state.wl_display)) {
        /* This space deliberately left blank */
    }
	
    return 0;
}