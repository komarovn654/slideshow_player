#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <string.h>
#include <ini.h>

#include "ssp_observer_ps.h"
#include "ssp_window.h"
#include "ssp_list.h"
#include "ssp_memory.h"

typedef struct {
    size_t window_width;
    size_t window_heigth;
    double switching_time;
    char dirs[SSP_OBS_DIRS_MAX_COUNT][SSP_PATH_MAX_LEN];
    size_t dirs_count;
} config;

static int ssp_config_handler(void* user, const char* section, const char* name, const char* value)
{
    config* pconfig = (config*)user;

    #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
    if (MATCH("Settings", "window_width")) {
        pconfig->window_width = atoi(value);
    } else if (MATCH("Settings", "window_heigth")) {
        pconfig->window_heigth = atoi(value);
    } else if (MATCH("Settings", "switching_time")) {
        pconfig->switching_time = atof(value);
    } else if (MATCH("Directories", "dirs")) {
        size_t start_ptr = 0, end_ptr = 0;
        pconfig->dirs_count = 0;

        int len = strlen(value);
        char* dirs = ssp_malloc(len);
        memcpy(dirs, value, len);

        while (start_ptr < len) {
            end_ptr = strcspn(dirs + start_ptr, ",");
            strncpy(pconfig->dirs[pconfig->dirs_count], dirs + start_ptr, end_ptr);
            start_ptr += end_ptr + 1;
            pconfig->dirs_count++;
        }
        ssp_free(dirs);
    } else {
        return 0;  /* unknown section/name, error */
    }
    return 1;

    return 0;
}

static int ssp_observer_init(config* ssp_config, ssp_image_storage* is)
{
    ssp_observer settings = {
        .dirs_count = ssp_config->dirs_count,
        .filter = ssp_is_file_image,
        .istorage = is,
    };

    for (size_t i = 0; i < ssp_config->dirs_count; i++) {
        settings.dirs[i] = (char*)malloc(SSP_PATH_MAX_LEN);
        snprintf(settings.dirs[i], SSP_PATH_MAX_LEN, "%s", ssp_config->dirs[i]);
    }

    return ssp_obsps_init(settings);
}

int main(int argc, char *argv[])
{
    openlog("slideshow_player", LOG_PID | LOG_PERROR, LOG_USER);

    config ssp_config = { 0 };

    if (ini_parse("../../config.ini", ssp_config_handler, &ssp_config) < 0) {
        syslog(LOG_ERR, "Can't load 'config.ini'");
        return 1;
    }
    
    ssp_image_storage* is = ssp_list_init_is();
    if (is == NULL) {
        return EXIT_FAILURE;
    }

    if (ssp_observer_init(&ssp_config, is) != 0) {
        return EXIT_FAILURE;
    }

    if (ssp_window_init(ssp_config.window_width, ssp_config.window_heigth, ssp_config.switching_time, is) != 0) {
        return EXIT_FAILURE;
    }
    
    
    while (ssp_window_player_loop() == 0) {
        ssp_obsps_process();
    }

    // ssp_window_destruct(main_window);
    return EXIT_SUCCESS;
}
