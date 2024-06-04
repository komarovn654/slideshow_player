#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>

#include "ssp_helper.h"

#define SSP_OBS_DIRS_MAX_COUNT  (10)

typedef struct ssp_observer {
    char* dirs[SSP_OBS_DIRS_MAX_COUNT];
    size_t dirs_count;
    ssp_image_storage* istorage;
    bool (*filter)(const char *file_name);
} ssp_observer;

ssp_observer* ssp_obs_init(ssp_observer settings);
void ssp_obs_destruct(ssp_observer* obs);
int ssp_obs_dirs_create(ssp_observer* obs);
int ssp_obs_dirs_traversal(ssp_observer* obs);
void* ssp_obs_storage_insert(ssp_observer* obs, const char* item_name);
void ssp_obs_storage_remove(ssp_observer* obs, const char* item_name);
bool ssp_obs_filter(ssp_observer* obs, const char *file_name);

#ifdef __cplusplus
}
#endif
