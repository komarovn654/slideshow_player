#include <stdlib.h>

#include "logman/logman.h"

#include "ssp_observer.h"
#include "ssp_window.h"
#include "ssp_render.h"
#include "ssp_image_loader.h"

static void logman_error_callback(void)
{
    fprintf(stderr, "%s\n", log_get_internal_error());
}

static logman_error ssp_logman_init(void)
{
    logman_settings settings = {
        .type = LOGTYPE_DEBUG,
        .out_type = LOGOUT_STREAM,
        .output.out_stream = stderr,
        .error_callback = logman_error_callback
    };

    logman_error err = log_init(&settings);
    if (log_init(&settings) != LOGERR_NOERR) {
        fprintf(stderr, "logman initialization error: %d\n", err);
        return err;
    }

    return LOGERR_NOERR;
}

int main(int argc, char *argv[])
{
    if (ssp_logman_init() != LOGERR_NOERR) {
        return EXIT_FAILURE;
    }

    if (ssp_image_loader_init() != 0) {
        return EXIT_FAILURE;
    }

    if (ssp_obs_init("/home/nikolay/base_images/") != 0) {
        return EXIT_FAILURE;
    }

    if (ssp_glfw_init(DP_X11) != 0) {
        return EXIT_FAILURE;
    }

    ssp_window main_window =  ssp_window_init(400, 400, 1.0, ssp_obs_images());
    if (main_window == NULL) {
        log_error("window initialization error");
    }

    if (ssp_render_init() != 0) {
        return EXIT_FAILURE;
    }
    
    
    while (ssp_player_loop(main_window)) {
        ssp_obs_process();
    }

    ssp_window_destruct(main_window);
    return EXIT_SUCCESS;
}
