#include <GLFW/glfw3.h>

#include "ssp_window.h"

static struct ssp_window_t {
    GLFWwindow *window;
} window;

ssp_window ssp_window_init(int width, int height)
{
    return &window;
}
