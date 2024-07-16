#include <stdio.h>
#include <syslog.h>

#include "ssp_observer.h"
#include "ssp_helper.h"
#include "ssp_memory.h"

ssp_static int ssp_obs_assert(ssp_observer settings)
{
    if (settings.dirs[0] == NULL || settings.dirs_count == 0) {
        syslog(LOG_ERR, "<Observer>: <Assert>: Unable to initialize the observer without directories");
        return 1;
    }

    if (settings.dirs_count > SSP_OBS_DIRS_MAX_COUNT) {
        syslog(LOG_ERR, "<Observer>: <Assert>: Too much directories to observe: %lu", settings.dirs_count);
        return 1;
    }    

    if (settings.istorage == NULL || 
        settings.istorage->insert == NULL || 
        settings.istorage->remove == NULL ||
        settings.istorage->storage == NULL) {
            syslog(LOG_ERR, 
                "<Observer>: <Assert>: Unable to initialize the observer without an item storage or methods for it");
            return 1;
    }

    if (settings.filter == NULL) {
        syslog(LOG_ERR, "<Observer>: <Assert>: Unable to initialize the observer without a filter function");
        return 1;
    }

    return 0;
}

int ssp_obs_dirs_create(ssp_observer* obs)
{
    for (size_t i = 0; i < obs->dirs_count; i++) {
        if (ssp_dir_create(obs->dirs[i]) != 0) {
            syslog(LOG_ERR, "<Observer>: <Create directories>: Observer directory wasn't created: %s", obs->dirs[i]);
            return 1;
        }
    }

    return 0;
}

ssp_observer* ssp_obs_init(ssp_observer settings)
{
    if (ssp_obs_assert(settings) != 0) {
        syslog(LOG_CRIT, "<Observer>: <Initialization>: Assertion error");
        return NULL;
    }

    ssp_observer* obs = (ssp_observer*)ssp_malloc(sizeof(ssp_observer));
    if (obs == NULL) {
        syslog(LOG_CRIT, "<Observer>: <Initialization>: Failed to allocate memory for the observer object");
        return NULL;
    }

    obs->dirs_count = settings.dirs_count;
    obs->istorage = settings.istorage;
    obs->filter = settings.filter;

    for (size_t i = 0; i < settings.dirs_count; i++) {
        if ((obs->dirs[i] = (char*)ssp_calloc(SSP_PATH_MAX_LEN, sizeof(char))) == NULL) {
            syslog(LOG_CRIT, "<Observer>: <Initialization>: Failed to allocate memory for the directory <%lu> name", i);
            return NULL;
        }
        snprintf(obs->dirs[i], SSP_PATH_MAX_LEN, "%s", settings.dirs[i]);
    }

    syslog(LOG_INFO, "<Observer>: <Initialization>: The observer was initialized");
    return obs;
}

void ssp_obs_destruct(ssp_observer* obs)
{
    for (size_t i = 0; i < obs->dirs_count; i++) {
        ssp_free(obs->dirs[i]);
    }

    ssp_free(obs);
    syslog(LOG_INFO, "<Observer>: <Destruct>: Observer was destructed");
}

void* ssp_obs_storage_insert(ssp_observer* obs, const char* item_name)
{
    if (item_name == NULL) {
        syslog(LOG_WARNING,  "<Observer>: <Insert item >: Unable to insert NULL item");
        return NULL;
    }

    if (obs == NULL) {
        syslog(LOG_WARNING,  "<Observer>: <Insert item >: Observer wasn't initialized; %s wasn't removed", item_name);
        return NULL;
    }

    return obs->istorage->insert(obs->istorage->storage, item_name);
}

void ssp_obs_storage_remove(ssp_observer* obs, const char* item_name)
{
    if (item_name == NULL) {
        syslog(LOG_WARNING,  "<Observer>: <Insert item >: Unable to remove NULL item");
        return;
    }

    if (obs == NULL) {
        syslog(LOG_WARNING, "<Observer>: <Remove item>: Observer wasn't initialized; %s wasn't removed", item_name);
        return;
    }

    obs->istorage->remove(&obs->istorage->storage, item_name);
}

bool ssp_obs_filter(ssp_observer* obs, const char *file_name)
{
    if (file_name == NULL) {
        syslog(LOG_WARNING,  "<Observer>: <Insert item >: Unable to filter NULL item");
        return false;
    }

    if (obs == NULL) {
        syslog(LOG_WARNING, "<Observer>: <Filter>: Observer wasn't initialized; %s wasn't filtred", file_name);
        return false;
    }

    return obs->filter(file_name);
}

int ssp_obs_dirs_traversal(ssp_observer* obs)
{
    if (obs == NULL) {
        syslog(LOG_WARNING, "<Observer>: <Traversal>: Observer wasn't initialized");
        return 1;
    }

    for (size_t i = 0; i < obs->dirs_count; i++) {
        if (ssp_dir_traversal(obs->dirs[i], obs->istorage->insert, obs->istorage->storage, obs->filter) != 0) {
            syslog(LOG_WARNING, "<Observer>: <Traversal>: Observer couldn't traversal directory <%s>", obs->dirs[i]);
            return 1;
        }
    }

    return 0;
}
