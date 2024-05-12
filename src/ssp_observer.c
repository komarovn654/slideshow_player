#include "logman/logman.h"
#include "ssp_observer.h"
#include "ssp_helper.h"
#include "ssp_memory.h"

ssp_static int ssp_obs_assert(observer settings)
{
    if (settings.dirs == NULL || settings.dirs[0] == NULL || settings.dirs_count == 0) {
        log_error("It's impossible to initialize the observer without directories");
        return 1;
    }

    if (settings.dirs_count > SSP_OBS_DIRS_MAX_COUNT) {
        log_error("There is too much directories to observe");
        return 1;
    }    

    if (settings.storage == NULL || settings.storage_insert == NULL || settings.storage_remove == NULL) {
        log_error("It's impossible to initialize the observer without an item storage or methods for it");
        return 1;
    }

    if (settings.filter == NULL) {
        log_error("It's impossible to initialize the observer without a filter function");
        return 1;
    }

    return 0;
}

int ssp_obs_dirs_create(observer* obs)
{
    for (size_t i = 0; i < obs->dirs_count; i++) {
        if (ssp_dir_create(obs->dirs[i]) != 0) {
            log_error("Observer directory wasn't created: %s", obs->dirs[i]);
            return 1;
        }
    }

    return 0;
}

observer* ssp_obs_init(observer settings)
{
    if (ssp_obs_assert(settings) != 0) {
        return NULL;
    }

    observer* obs = (observer*)ssp_malloc(sizeof(observer));
    if (obs == NULL) {
        log_error("Observer memory allocation error");
        return NULL;
    }

    obs->dirs_count = settings.dirs_count;
    obs->storage = settings.storage;
    obs->storage_insert = settings.storage_insert;
    obs->storage_remove = settings.storage_remove;
    obs->filter = settings.filter;

    for (size_t i = 0; i < settings.dirs_count; i++) {
        if ((obs->dirs[i] = (char*)ssp_calloc(SSP_OBS_DIR_NAME_LEN, 1)) == NULL) {
            log_error("Observer memory allocation error");
            return NULL;
        }
        snprintf(obs->dirs[i], SSP_OBS_DIR_NAME_LEN, "%s", settings.dirs[i]);
    }

    return obs;
}

void ssp_obs_destruct(observer* obs)
{
    for (size_t i = 0; i < obs->dirs_count; i++) {
        ssp_free(obs->dirs[i]);
    }

    ssp_free(obs);
}

void* ssp_obs_storage_insert(observer* obs, const char* item_name)
{
    if (obs == NULL) {
        log_error("Observer wasn't initialized");
        return NULL;
    }

    return obs->storage_insert(obs->storage, item_name);
}

void ssp_obs_storage_remove(observer* obs, const char* item_name)
{
    obs->storage_remove(obs->storage, item_name);
}

bool ssp_obs_filter(observer* obs, const char *file_name)
{
    return obs->filter(file_name);
}

int ssp_obs_dirs_traversal(observer* obs)
{
    for (size_t i = 0; i < obs->dirs_count; i++) {
        if (ssp_dir_traversal(obs->dirs[i], obs->storage_insert, obs->storage, obs->filter) != 0) {
            log_error("Observer couldn't traversal directory <%s>", obs->dirs[i]);
            return 1;
        }
    }

    return 0;
}