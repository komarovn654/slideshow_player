#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <string.h>

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
} ssp_window;

ssp_static void ssp_glfw_error_callback(int error, const char* description)
{
    log_error("GLFW Error [%i]: %s\n", error, description);
}

ssp_static void ssp_window_resize_handler()
{
    glfwGetFramebufferSize(ssp_window.window, &ssp_window.width_pixels, &ssp_window.height_pixels);
    // glViewport(0, 0, window->width_pixels, window->height_pixels);
    log_debug("Window have been resized to %ix%i", ssp_window.width, ssp_window.height);
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
    log_debug("%s platform has been set", platform_name);

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
    log_debug("GLFW has been initializated for %s", platform_name);

    return 0;
}

int ssp_window_init(int width, int height, double redraw_time, ssp_image_storage* images)
{
    if ((width <= 0 || width > MAX_WINDOW_WIDTH) || (height <= 0 || height > MAX_WINDOW_HEIGHT)) {
        log_error("required width in range [%i..%i] and height in range [%i..%i]", 0, MAX_WINDOW_WIDTH, 0, MAX_WINDOW_HEIGHT);
        return 1;
    }

    if (ssp_glfw_init(SSP_DP_COCOA) != 0) {
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
    
    ssp_window_resize_handler();
    glfwMakeContextCurrent(ssp_window.window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        log_error("GLAD initialization error");
        return 1;
    }

    printf("OpenGL ES Version: %s\n", glGetString(GL_VERSION));

    if (ssp_render_init() != 0) {
        log_error("Render initialization error");
        return 1;        
    }
    
    ssp_window.images = images;
    
    printf("%s\n", glGetString(GL_VERSION));
    printf("%s\n", glGetString(GL_RENDERER));
    printf("%s\n", glGetString(GL_VENDOR));

    return 0;
}

void ssp_window_destruct()
{
    glfwTerminate();
}

int ssp_window_player_loop(void)
{   
    if (glfwWindowShouldClose(ssp_window.window)) {
        log_info("Window have been closed");
        return 1;
    }
    
    if (ssp_window_needs_to_redraw()) {
        glfwMakeContextCurrent(ssp_window.window);
        
        char* image = ssp_window.images->image_name(ssp_window.images->storage_ptr);
        if (ssp_render_redraw(image) != 0) {
            log_error("Redraw error: %s", image);
            return 1;
        }

        glfwSwapBuffers(ssp_window.window);
        log_info("Readrawed: %s", image);

        ssp_window.images->storage_ptr = ssp_window.images->move_ptr_to_next(ssp_window.images->storage_ptr);
        if (ssp_window.images->storage_ptr == NULL) {
            ssp_window.images->storage_ptr = ssp_window.images->storage_head;
        }
    }

    glfwPollEvents();
    return 0;
}
