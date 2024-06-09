#include <stdlib.h>

#include "logman/logman.h"

#include "ssp_observer_ps.h"
#include "ssp_window.h"
#include "ssp_list.h"

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

static int ssp_observer_init(char* dirs[], size_t dirs_count, ssp_image_storage* is)
{
    ssp_observer settings = {
        .dirs_count = dirs_count,
        .filter = ssp_is_file_image,
        .istorage = is,
    };
    for (size_t i = 0; i < dirs_count; i++) {
        settings.dirs[i] = (char*)malloc(SSP_PATH_MAX_LEN);
        snprintf(settings.dirs[i], SSP_PATH_MAX_LEN, "%s", dirs[i+2]);
    }

    return ssp_obsps_init(settings);
}

int main(int argc, char *argv[])
{
    if (ssp_logman_init() != LOGERR_NOERR) {
        return EXIT_FAILURE;
    }

    ssp_image_storage* is = ssp_list_init_is();
    if (is == NULL) {
        return EXIT_FAILURE;
    }

    if (ssp_observer_init(argv, (size_t)argv[1][0] - (size_t)'0', is) != 0) {
        return EXIT_FAILURE;
    }

    if (ssp_window_init(800, 400, 1.0, is) != 0) {
        return EXIT_FAILURE;
    }
    
    
    while (ssp_window_player_loop() == 0) {
        ssp_obsps_process();
    }

    // ssp_window_destruct(main_window);
    return EXIT_SUCCESS;
}
