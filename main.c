#include <logger.h>
#include <stdlib.h>

#include "ssp_window.h"

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

    ssp_window window =  ssp_window_init(400, 800);
    if (window == NULL) {
        log_panic("window initialization error");
    }

    return EXIT_SUCCESS;
}
