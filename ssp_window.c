// #include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <logger.h>
#include <string.h>

#include "ssp_window.h"
#include "ssp_render.h"

typedef struct ssp_window_t {
    GLFWwindow *window;
    int width, height, width_pixels, height_pixels;

    double redraw_time;
} ssp_window_t;

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error: %s\n", description);
}

static void resize_handler(ssp_window_t *window)
{
    glfwGetFramebufferSize(window->window, &window->width_pixels, &window->height_pixels);
    glViewport(0, 0, window->width_pixels, window->height_pixels);
    log_debug("window have been resized to %ix%i", window->width, window->height);
}

static void ssp_display_platform_name(int glfw_platform_id, char *name, int name_size) {
    switch (glfw_platform_id) {
    case GLFW_PLATFORM_WAYLAND:
        strncpy(name, "wayland", name_size);
        break;
    case GLFW_PLATFORM_COCOA:
        strncpy(name, "cocoa", name_size);
        break;
    case GLFW_PLATFORM_X11:
        strncpy(name, "x11", name_size);
        break;    
    default:
        log_warning("unknown display platform type");
        break;
    }
}

int static ssp_display_platform_set(ssp_display_platform platform) {
    char platform_name[10];
    ssp_display_platform_name(platform, platform_name, sizeof(platform_name));

    if (glfwPlatformSupported(platform) != 1) {
        log_error("%s platform doesn't support", platform_name);
        return -1;
    }   

    glfwInitHint(GLFW_PLATFORM, platform);
    log_debug("%s platform has been set", platform_name);

    return 0;
}

bool static ssp_needs_to_redraw(ssp_window_t *window)
{
    if (glfwGetTime() >= window->redraw_time) {
        glfwSetTime(0);
        return true;
    }

    return false;
}

int ssp_glfw_init(ssp_display_platform platform)
{
    glfwSetErrorCallback(error_callback);

    log_debug("GLFW runtime  version: %s", glfwGetVersionString());
    log_debug("GLFW compiled version: %i.%i.%i", GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR, GLFW_VERSION_REVISION);

    if (ssp_display_platform_set(platform) != 0) {
        log_error("couldn't set display platform");
        return -1;
    }

    if (!glfwInit()) {
        log_error("GLFW initialization error");
        return -1;
    }

    char platform_name[10];
    ssp_display_platform_name(glfwGetPlatform(), platform_name, sizeof(platform_name));
    log_debug("GLFW has been initializated for %s", platform_name);

    return 0;
}

static void ssp_context_set_gl33(void)
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

static void ssp_context_set_gles20(void)
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

ssp_window ssp_window_init(int width, int height, double redraw_time)
{
    if ((width <= 0 || width > MAX_WINDOW_WIDTH) || (height <= 0 || height > MAX_WINDOW_HEIGHT)) {
        log_error("required width in range [%i..%i] and height in range [%i..%i]", 0, MAX_WINDOW_WIDTH, 0, MAX_WINDOW_HEIGHT);
        return NULL;
    }

    ssp_window_t *window = (ssp_window_t *)malloc(sizeof(ssp_window_t));
    window->width = width;
    window->height = height;
    window->redraw_time = redraw_time;

    window->window = glfwCreateWindow(window->width, window->height, "ssp", NULL, NULL); // glfwGetPrimaryMonitor()
    if (window->window == NULL) {
        log_error("GLFW couldn't create window");
        return NULL;
    }
    
    ssp_context_set_gles20();
    resize_handler(window);
    glfwMakeContextCurrent(window->window);
    return window;
}

void ssp_window_destruct(ssp_window window)
{
    glfwTerminate();
    free(window);
}

int ssp_player_loop(ssp_window window)
{
    glfwMakeContextCurrent(window->window);
    glfwSetTime(window->redraw_time);

    while (1) {
        if (glfwWindowShouldClose(window->window)) {
            log_info("window have been closed");
            break;
        }

        if (ssp_needs_to_redraw(window)) {
            ssp_redraw();

            glfwSwapBuffers(window->window);
            log_info("redrawed");
        }
        
        glfwPollEvents();
    }

    return 0;
}
