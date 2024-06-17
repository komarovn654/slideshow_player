#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

#include "ssp_observer_ps.h"
#include "ssp_window.h"
#include "ssp_list.h"

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
    openlog("slideshow_player", LOG_PID | LOG_PERROR, LOG_USER);
    setlogmask(LOG_EMERG | LOG_ALERT | LOG_CRIT | LOG_ERR | LOG_WARNING | LOG_NOTICE | LOG_INFO | LOG_DEBUG);

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
