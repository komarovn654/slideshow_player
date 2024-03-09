#include <logger.h>
#include <stdlib.h>

#include "ssp_window.h"
#include "ssp_render.h"
#include "ssp_image_loader.h"

static void logger_error_callback(void)
{
    fprintf(stderr, "%s\n", log_get_internal_error());
}

static logger_error ssp_logger_init(void)
{
    logger_settings settings = {
        .type = LOGTYPE_DEBUG,
        .out_type = LOGOUT_STREAM,
        .output.out_stream = stderr,
        .error_callback = logger_error_callback
    };

    logger_error err = log_init(&settings);
    if (log_init(&settings) != LOGERR_NOERR) {
        fprintf(stderr, "logger initialization error: %d\n", err);
        return err;
    }

    return LOGERR_NOERR;
}

int main(int argc, char *argv[])
{
    if (ssp_logger_init() != LOGERR_NOERR) {
        return EXIT_FAILURE;
    }

    if (ssp_image_loader_init() != 0) {
        return EXIT_FAILURE;
    }

    if (ssp_glfw_init(DP_X11) != 0) {
        return EXIT_FAILURE;
    }

    ssp_window main_window =  ssp_window_init(400, 800, 3.0);
    if (main_window == NULL) {
        log_panic("window initialization error");
    }

    if (ssp_render_init() != 0) {
        return EXIT_FAILURE;
    }

    ssp_player_loop(main_window);

    ssp_window_destruct(main_window);
    return EXIT_SUCCESS;
}
