#include <stdio.h>
#include <syslog.h>

#include "ssp_observer.h"
#include "ssp_helper.h"
#include "ssp_memory.h"

ssp_static int ssp_obs_assert(ssp_observer settings)
{
    if (settings.dirs[0] == NULL || settings.dirs_count == 0) {
        syslog(LOG_ERR, "SSP Observer. It's impossible to initialize the observer without directories");
        return 1;
    }

    if (settings.dirs_count > SSP_OBS_DIRS_MAX_COUNT) {
        syslog(LOG_ERR, "SSP Observer. There is too much directories to observe");
        return 1;
    }    

    if (settings.istorage == NULL || 
        settings.istorage->insert == NULL || 
        settings.istorage->remove == NULL ||
        settings.istorage->storage == NULL) {
            syslog(LOG_ERR, 
                "SSP Observer. It's impossible to init the observer without an itemstorage or methods for it");
            return 1;
    }

    if (settings.filter == NULL) {
        syslog(LOG_ERR, "SSP Observer. It's impossible to initialize the observer without a filter function");
        return 1;
    }

    return 0;
}

int ssp_obs_dirs_create(ssp_observer* obs)
{
    for (size_t i = 0; i < obs->dirs_count; i++) {
        if (ssp_dir_create(obs->dirs[i]) != 0) {
            syslog(LOG_ERR, "SSP Observer. Observer directory wasn't created: %s", obs->dirs[i]);
            return 1;
        }
    }

    return 0;
}

ssp_observer* ssp_obs_init(ssp_observer settings)
{
    if (ssp_obs_assert(settings) != 0) {
        return NULL;
    }

    ssp_observer* obs = (ssp_observer*)ssp_malloc(sizeof(ssp_observer));
    if (obs == NULL) {
        syslog(LOG_ERR, "SSP Observer. Observer memory allocation error");
        return NULL;
    }

    obs->dirs_count = settings.dirs_count;
    obs->istorage = settings.istorage;
    obs->filter = settings.filter;

    for (size_t i = 0; i < settings.dirs_count; i++) {
        if ((obs->dirs[i] = (char*)ssp_calloc(SSP_PATH_MAX_LEN, sizeof(char))) == NULL) {
            syslog(LOG_ERR, "SSP Observer. Directory name <%lu> allocation error", i);
            return NULL;
        }
        snprintf(obs->dirs[i], SSP_PATH_MAX_LEN, "%s", settings.dirs[i]);
    }

    syslog(LOG_INFO, "SSP Observer. The observer was initialized");
    return obs;
}

void ssp_obs_destruct(ssp_observer* obs)
{
    for (size_t i = 0; i < obs->dirs_count; i++) {
        ssp_free(obs->dirs[i]);
    }

    ssp_free(obs);
    syslog(LOG_INFO, "SSP Observer. Observer was destructed");
}

void* ssp_obs_storage_insert(ssp_observer* obs, const char* item_name)
{
    if (obs == NULL) {
        syslog(LOG_ERR, "SSP Observer. Observer wasn't initialized");
        return NULL;
    }

    return obs->istorage->insert(obs->istorage->storage, item_name);
}

void ssp_obs_storage_remove(ssp_observer* obs, const char* item_name)
{
    if (obs == NULL) {
        syslog(LOG_ERR, "SSP Observer. Observer wasn't initialized");
    }

    obs->istorage->remove(&obs->istorage->storage, item_name);
}

bool ssp_obs_filter(ssp_observer* obs, const char *file_name)
{
    if (obs == NULL) {
        syslog(LOG_ERR, "SSP Observer. Observer wasn't initialized");
        return false;
    }

    return obs->filter(file_name);
}

int ssp_obs_dirs_traversal(ssp_observer* obs)
{
    if (obs == NULL) {
        syslog(LOG_ERR, "SSP Observer. Observer wasn't initialized");
        return 1;
    }

    for (size_t i = 0; i < obs->dirs_count; i++) {
        if (ssp_dir_traversal(obs->dirs[i], obs->istorage->insert, obs->istorage->storage, obs->filter) != 0) {
            syslog(LOG_ERR, "SSP Observer. Observer couldn't traversal directory <%s>", obs->dirs[i]);
            return 1;
        }
    }

    return 0;
}
