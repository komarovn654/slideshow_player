#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <string.h>
#include <ini.h>

#include "ssp_observer_ps.h"
#include "ssp_window.h"
#include "ssp_list.h"

typedef struct {
    size_t window_width;
    size_t window_heigth;
    double switching_time;
    char dirs[SSP_OBS_DIRS_MAX_COUNT][SSP_FULL_NAME_MAX_LEN];
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
        char* token;
        char* rest = strdup(value);
        pconfig->dirs_count = 0;
        while ((token = strtok_r(rest, ",", &rest)) && pconfig->dirs_count < SSP_OBS_DIRS_MAX_COUNT) {
            strncpy(pconfig->dirs[pconfig->dirs_count], token, SSP_FULL_NAME_MAX_LEN);
            pconfig->dirs_count++;
        }
        free(rest);
    } else {
        return 0;  /* unknown section/name, error */
    }
    return 1;

    return 0;
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
    openlog("slideshow_player", LOG_PID | LOG_PERROR, LOG_USER);
    setlogmask(LOG_EMERG | LOG_ALERT | LOG_CRIT | LOG_ERR | LOG_WARNING | LOG_NOTICE | LOG_INFO | LOG_DEBUG);

    config ssp_config = { 0 };

    if (ini_parse("../../config.ini", ssp_config_handler, &ssp_config) < 0) {
        syslog(LOG_ERR, "Can't load 'config.ini'");
        return 1;
    }
    
    ssp_image_storage* is = ssp_list_init_is();
    if (is == NULL) {
        return EXIT_FAILURE;
    }

    if (ssp_observer_init((char**)ssp_config.dirs, ssp_config.dirs_count, is) != 0) {
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
