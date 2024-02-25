#include <stdio.h>
#include "logger.h"

#include "ssp_display.h"
#include "ssp_egl.h"

#include <stdio.h>
#include <unistd.h>

static void logger_error_callback(void)
{
    fprintf(stderr, "%s\n", log_get_internal_error());
}

static int ssp_logger_init(void)
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
        return 1;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    if (ssp_logger_init() != 0) {
        return 1;
    }

    if (ssp_init_display() != 0) {
        log_panic("display initialization error");
    }

    // init_image_loader

    // draw_2();

    while(1) {
        // get image
        draw_1();
        // render image
        sleep(3);
        // commit surface
        draw_2();
        sleep(3);
        // sleep
    }

    return 0;
}