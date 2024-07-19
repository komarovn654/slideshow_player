#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ssp_observer_ps.h"
#include "ssp_test_storage.h"
#include "ssp_helper.h"

static bool filter_function(const char* item_name)
{
    const char txt_file[] = ".txt";

    if (strstr(item_name, txt_file) != NULL) {
        return true;
    }

    return false;
}

static int ssp_observer_init(char* dirs[], size_t dirs_count)
{
    ssp_observer obs = {
        .dirs_count = dirs_count,
        .filter = filter_function,
        .istorage = ssp_test_storage_init_is(),
    };
    for (size_t i = 0; i < dirs_count; i++) {
        obs.dirs[i] = (char*)malloc(SSP_PATH_MAX_LEN);
        snprintf(obs.dirs[i], SSP_PATH_MAX_LEN, "%s", dirs[i+2]);
    }

    int result = ssp_obsps_init(obs);

    for (size_t i = 0; i < dirs_count; i++) {
        free(obs.dirs[i]);
    }
    return result;
}

int main(int argc, char *argv[])
{
    int result = EXIT_SUCCESS;

    if (argc < 3) {
        ssp_syslog(LOG_ERR, "usage: ssp_observer_mtest <dirs_count> <dir1_to_observ> <dir2_to_observ> ...up to 10 dirs");
        return EXIT_FAILURE;
    }

    if (ssp_observer_init(argv, (size_t)argv[1][0] - (size_t)'0') != 0) {
        ssp_syslog(LOG_ERR, "Observer initialization error");
        return EXIT_FAILURE;
    };

    while(1) {
        if (ssp_obsps_process() != 0) {
            ssp_syslog(LOG_ERR, "Observer crashed");
            return EXIT_FAILURE;
        };
    }

    return EXIT_SUCCESS;
}
