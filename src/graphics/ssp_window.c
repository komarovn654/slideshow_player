#include <string.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "ssp_image_loader.h"
#include "ssp_helper.h"
#include "ssp_window.h"
#include "ssp_render.h"

ssp_static int ssp_glfw_init(ssp_display_platform platform);
ssp_static void ssp_window_resize_handler(int image_width, int image_height);

static ssp_window_t ssp_window;

static ssp_glfw_init_t __ssp_glfw_init = ssp_glfw_init;
static ssp_glfw_set_time_t __ssp_glfw_set_time = glfwSetTime;
static ssp_render_set_gl_ctx_t __ssp_render_set_gl_ctx = ssp_render_set_gl_ctx;
static ssp_glfw_create_window_t __ssp_glfw_create_window = glfwCreateWindow;
static ssp_glfw_make_context_current_t __ssp_glfw_make_context_current = glfwMakeContextCurrent;
static ssp_render_init_t __ssp_render_init = ssp_render_init;
static ssp_window_resize_handler_t __ssp_window_resize_handler = ssp_window_resize_handler;

void ssp_ws_glfw_init(ssp_glfw_init_t f_ptr) { __ssp_glfw_init = f_ptr; }
void ssp_ws_glfw_time(ssp_glfw_set_time_t f_ptr) { __ssp_glfw_set_time = f_ptr; }
void ssp_ws_render_set_gl_ctx(ssp_render_set_gl_ctx_t f_ptr) { __ssp_render_set_gl_ctx = f_ptr; }
void ssp_ws_glfw_create_window(ssp_glfw_create_window_t f_ptr) { __ssp_glfw_create_window = f_ptr; }
void ssp_ws_glfw_make_context_current(ssp_glfw_make_context_current_t f_ptr) { __ssp_glfw_make_context_current = f_ptr; }
void ssp_ws_render_init(ssp_render_init_t f_ptr) { __ssp_render_init = f_ptr; }
void ssp_ws_window_resize_handler(ssp_window_resize_handler_t f_ptr) { __ssp_window_resize_handler = f_ptr; }

void ssp_ws_default_fptr(void)
{
    __ssp_glfw_init = ssp_glfw_init;
    __ssp_glfw_set_time = glfwSetTime;
    __ssp_render_set_gl_ctx = ssp_render_set_gl_ctx;
    __ssp_glfw_create_window = glfwCreateWindow;
    __ssp_glfw_make_context_current = glfwMakeContextCurrent;
    __ssp_render_init = ssp_render_init;
}

ssp_window_t *ssp_window_get_window(void)
{
    return &ssp_window;
}

ssp_static void ssp_glfw_error_callback(int error, const char* description)
{
    ssp_syslog(LOG_ERR, "SSP. GLFW Error [%i]: %s\n", error, description);
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
    ssp_syslog(LOG_DEBUG, "SSP. Image has been resized to %ix%i", res_width, res_height);
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
        ssp_syslog(LOG_WARNING, "SSP. An unknown display platform type");
        break;
    }
}

ssp_static int ssp_window_set_platform(ssp_display_platform platform) {
    char platform_name[10];
    ssp_window_set_platform_name(platform, platform_name, sizeof(platform_name));

    if (glfwPlatformSupported(platform) != 1) {
        ssp_syslog(LOG_ERR, "SSP. %s platform doesn't support", platform_name);
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

    ssp_syslog(LOG_INFO, "SSP. GLFW runtime  version: %s", glfwGetVersionString());
    ssp_syslog(LOG_INFO, "SSP. GLFW compiled version: %i.%i.%i", 
        GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR, GLFW_VERSION_REVISION);

    if (ssp_window_set_platform(platform) != 0) {
        ssp_syslog(LOG_ERR, "SSP. Couldn't set display platform");
        return 1;
    }

    if (!glfwInit()) {
        ssp_syslog(LOG_ERR, "SSP. GLFW initialization error");
        return 1;
    }

    char platform_name[10];
    ssp_window_set_platform_name(glfwGetPlatform(), platform_name, sizeof(platform_name));
    ssp_syslog(LOG_INFO, "SSP. GLFW has been initializated for %s", platform_name);

    return 0;
}



int ssp_window_init(int width, int height, double redraw_time, ssp_image_storage* images)
{
    if ((width <= 0 || width > MAX_SSP_WINDOW_WIDTH) || (height <= 0 || height > MAX_SSP_WINDOW_HEIGHT)) {
        ssp_syslog(LOG_ERR, "SSP. Required width in range [0..%i] and height in range [0..%i]", 
            MAX_SSP_WINDOW_WIDTH, MAX_SSP_WINDOW_HEIGHT);
        return 1;
    }

    if (images == NULL) {
        return 2;
    }

    if (__ssp_glfw_init(SSP_DISPLAY_PLATFORM) != 0) {
        ssp_syslog(LOG_ERR, "SSP. GLFW initialization error");
        return 3;
    }

    ssp_window.width = width;
    ssp_window.height = height;
    ssp_window.redraw_time = redraw_time;
    __ssp_glfw_set_time(ssp_window.redraw_time);

    __ssp_render_set_gl_ctx();

    ssp_window.window = __ssp_glfw_create_window(ssp_window.width, ssp_window.height, "ssp", NULL, NULL); // glfwGetPrimaryMonitor()
    if (ssp_window.window == NULL) {
        ssp_syslog(LOG_ERR, "SSP. GLFW couldn't create window");
        return 4;
    }

    __ssp_glfw_make_context_current(ssp_window.window);

    if (__ssp_render_init(__ssp_window_resize_handler) != 0) {
        ssp_syslog(LOG_ERR, "SSP. Render initialization error");
        return 5;        
    }
    ssp_syslog(LOG_INFO, "SSP. OpenGL version: %s", glGetString(GL_VERSION));
    ssp_syslog(LOG_INFO, "SSP. The window was initialized");

    ssp_window.images = images;
    ssp_window.head_storage = ssp_window.images->storage;
    ssp_window.current_storage = ssp_window.images->storage;

    return 0;
}

void ssp_window_destruct()
{
    glfwTerminate();
    ssp_syslog(LOG_INFO, "SSP. Window was destructed");
}

int ssp_window_player_loop(void)
{   
    if (glfwWindowShouldClose(ssp_window.window)) {
        ssp_syslog(LOG_INFO, "SSP. Window was closed");
        return 1;
    }
    
    if (ssp_window_needs_to_redraw()) {
        glfwMakeContextCurrent(ssp_window.window);
        
        char* image = ssp_window.images->image_name(ssp_window.current_storage);
        if (ssp_render_redraw(image) != 0) {
            ssp_syslog(LOG_ERR, "SSP. Redraw error: %s", image);
            return 0;
        }

        glfwSwapBuffers(ssp_window.window);
        ssp_syslog(LOG_INFO, "SSP. Readrawed: %s", image);

        ssp_window.current_storage = ssp_window.images->move_to_next(ssp_window.current_storage);
        if (ssp_window.current_storage == NULL) {
            ssp_window.current_storage = ssp_window.head_storage;
        }
    }

    glfwPollEvents();
    return 0;
}
