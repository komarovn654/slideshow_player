#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>

#include "ssp_helper.h"

#define SSP_OBS_DIRS_MAX_COUNT  (10)
#define SSP_OBS_DIR_NAME_LEN    (256)

typedef struct observer {
    char* dirs[SSP_OBS_DIRS_MAX_COUNT];
    size_t dirs_count;

    void* storage;
    void* (*storage_insert)(void*  storage, const char* item_name);
    void  (*storage_remove)(void** storage, const char* item_name);

    bool (*filter)(const char *file_name);
} observer;

observer* ssp_obs_init(observer settings);
void ssp_obs_destruct(observer* obs);
int ssp_obs_dirs_create(observer* obs);
int ssp_obs_dirs_traversal(observer* obs);
void* ssp_obs_storage_insert(observer* obs, const char* item_name);
void ssp_obs_storage_remove(observer* obs, const char* item_name);
bool ssp_obs_filter(observer* obs, const char *file_name);

#ifdef __cplusplus
}
#endif
