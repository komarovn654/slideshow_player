#include <string.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "logman/logman.h"

#include "ssp_image_loader.h"
#include "ssp_helper.h"
#include "ssp_window.h"
#include "ssp_render.h"

static struct ssp_window_t {
    GLFWwindow *window;
    int width, height, width_pixels, height_pixels;

    double redraw_time;
    ssp_image_storage* images;
    void* head_storage;
    void* current_storage;
} ssp_window;

ssp_static void ssp_glfw_error_callback(int error, const char* description)
{
    log_error("GLFW Error [%i]: %s\n", error, description);
}

ssp_static void ssp_window_calc_size(int image_width, int image_height, int* res_width, int* res_height)
{
    double ratio = (double)image_height / (double)ssp_window.height_pixels;

    *res_height = ssp_window.height_pixels;
    *res_width = (double)image_width / ratio;
}

ssp_static void ssp_window_resize_handler(int image_width, int image_height)
{
    glfwGetFramebufferSize(ssp_window.window, &ssp_window.width_pixels, &ssp_window.height_pixels);

    int res_width, res_height;
    ssp_window_calc_size(image_width, image_height, &res_width, &res_height);

    int x_offset = (ssp_window.width_pixels - res_width) / 2;
    glViewport(x_offset, 0, res_width, res_height);
    log_debug("Image has been resized to %ix%i", res_width, res_height);
}

ssp_static void ssp_window_set_platform_name(int glfw_platform_id, char *name, int name_size) {
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
    case GLFW_PLATFORM_WIN32:
        strncpy(name, "win32", name_size);
        break;         
    default:
        log_warning("unknown display platform type");
        break;
    }
}

ssp_static int ssp_window_set_platform(ssp_display_platform platform) {
    char platform_name[10];
    ssp_window_set_platform_name(platform, platform_name, sizeof(platform_name));

    if (glfwPlatformSupported(platform) != 1) {
        log_error("%s platform doesn't support", platform_name);
        return 1;
    }   

    glfwInitHint(GLFW_PLATFORM, platform);

    return 0;
}

ssp_static bool ssp_window_needs_to_redraw(void)
{
    if (glfwGetTime() >= ssp_window.redraw_time) {
        glfwSetTime(0);
        return true;
    }

    return false;
}

ssp_static int ssp_glfw_init(ssp_display_platform platform)
{
    glfwSetErrorCallback(ssp_glfw_error_callback);

    log_info("GLFW runtime  version: %s", glfwGetVersionString());
    log_info("GLFW compiled version: %i.%i.%i", GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR, GLFW_VERSION_REVISION);

    if (ssp_window_set_platform(platform) != 0) {
        log_error("Couldn't set display platform");
        return -1;
    }

    if (!glfwInit()) {
        log_error("GLFW initialization error");
        return -1;
    }

    char platform_name[10];
    ssp_window_set_platform_name(glfwGetPlatform(), platform_name, sizeof(platform_name));
    log_info("GLFW has been initializated for %s", platform_name);

    return 0;
}

int ssp_window_init(int width, int height, double redraw_time, ssp_image_storage* images)
{
    if ((width <= 0 || width > MAX_WINDOW_WIDTH) || (height <= 0 || height > MAX_WINDOW_HEIGHT)) {
        log_error("Required width in range [%i..%i] and height in range [%i..%i]", 0, MAX_WINDOW_WIDTH, 0, MAX_WINDOW_HEIGHT);
        return 1;
    }

    if (ssp_glfw_init(SSP_DISPLAY_PLATFORM) != 0) {
        log_error("SSP GLFW initialization error");
        return 1;
    }

    ssp_window.width = width;
    ssp_window.height = height;
    ssp_window.redraw_time = redraw_time;
    glfwSetTime(ssp_window.redraw_time);

    ssp_render_set_gl_ctx();

    ssp_window.window = glfwCreateWindow(ssp_window.width, ssp_window.height, "ssp", NULL, NULL); // glfwGetPrimaryMonitor()
    if (ssp_window.window == NULL) {
        log_error("GLFW couldn't create window");
        return 1;
    }
    
    glfwMakeContextCurrent(ssp_window.window);

    if (ssp_render_init(ssp_window_resize_handler) != 0) {
        log_error("Render initialization error");
        return 1;        
    }
    log_info("OpenGL version: %s", glGetString(GL_VERSION));

    ssp_window.images = images;
    ssp_window.head_storage = ssp_window.images->storage;
    ssp_window.current_storage = ssp_window.images->storage;

    return 0;
}

void ssp_window_destruct()
{
    glfwTerminate();
}

int ssp_window_player_loop(void)
{   
    if (glfwWindowShouldClose(ssp_window.window)) {
        log_info("Window has been closed");
        return 1;
    }
    
    if (ssp_window_needs_to_redraw()) {
        glfwMakeContextCurrent(ssp_window.window);
        
        char* image = ssp_window.images->image_name(ssp_window.current_storage);
        if (ssp_render_redraw(image) != 0) {
            log_error("Redraw error: %s", image);
            return 0;
        }

        glfwSwapBuffers(ssp_window.window);
        log_info("Readrawed: %s", image);

        ssp_window.current_storage = ssp_window.images->move_to_next(ssp_window.current_storage);
        if (ssp_window.current_storage == NULL) {
            ssp_window.current_storage = ssp_window.head_storage;
        }
    }

    glfwPollEvents();
    return 0;
}
