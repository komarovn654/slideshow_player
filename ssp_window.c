#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <logger.h>

#include "ssp_window.h"

typedef struct ssp_window_t {
    GLFWwindow *window;
    int width, height, width_pixels, height_pixels;
} ssp_window_t;

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error: %s\n", description);
}

static void resize_handler(ssp_window_t *window)
{
    glfwGetFramebufferSize(window->window, &window->width_pixels, &window->height_pixels);
    glViewport(0, 0, window->width_pixels, window->height_pixels);
    log_debug("window have been resized: %i %i", window->width, window->height);
}

int ssp_glfw_init(void)
{
    glfwSetErrorCallback(error_callback);

    log_debug("GLFW runtime version: %s", glfwGetVersionString());
    log_debug("GLFW compiled version: %i.%i.%i", GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR, GLFW_VERSION_REVISION);

    if (!glfwInit()) {
        log_error("GLFW initialization error");
        return -1;
    }

    return 0;
}

ssp_window ssp_window_init(int width, int height)
{
    if ((width <= 0 || width > MAX_WINDOW_WIDTH) || (height <= 0 || height > MAX_WINDOW_HEIGHT)) {
        log_error("required width in range [%i..%i] and height in range [%i..%i]", 0, MAX_WINDOW_WIDTH, 0, MAX_WINDOW_HEIGHT);
        return NULL;
    }

    ssp_window_t *window = (ssp_window_t *)malloc(sizeof(ssp_window_t));
    window->width = width;
    window->height = height;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window->window = glfwCreateWindow(window->width, window->height, "ssp", NULL, NULL);
    if (window->window == NULL) {
        log_error("GLFW couldn't create window");
        return NULL;
    }
    resize_handler(window);

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

    while (1) {
        if (glfwWindowShouldClose(window->window)) {
            log_info("window have been closed");
            break;
        }
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        glfwSwapBuffers(window->window);
        glfwPollEvents();
    }

    return 0;
}