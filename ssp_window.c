#include <GLFW/glfw3.h>
#include <logger.h>

#include "ssp_window.h"

static struct ssp_window_t {
    GLFWwindow *window;
    int width, height, width_pixels, height_pixels;

    void (*set_gl_type)(void);
} window;

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error: %s\n", description);
}

static void set_gl33(void)
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
}

ssp_window ssp_window_init(int width, int height)
{
    if ((width <= 0 || width > MAX_WINDOW_WIDTH) || (height <= 0 || height > MAX_WINDOW_HEIGHT)) {
        log_error("required width in range [%i..%i] and height in range [%i..%i]", 0, MAX_WINDOW_WIDTH, 0, MAX_WINDOW_HEIGHT);
        return NULL;
    }

    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        log_error("GLFW initialization error");
        return NULL;
    }

    window.width = width;
    window.height = height;


    return &window;
}
