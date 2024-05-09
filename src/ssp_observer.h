#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "ssp_helper.h"

#define SSP_OBS_DIRS_MAX_COUNT  (10)
#define SSP_OBS_DIR_NAME_LEN    (256)

typedef struct observer {
    char *dirs[SSP_OBS_DIR_NAME_LEN];
    size_t dirs_count;

    void *storage;
    void *(storage_insert)(void *storage, const char* item_name);
    void  (storage_delete)(void *storage, const char* item_name);
} observer;

void *ssp_obs_storage(void);
char **ssp_obs_dirs(void);
size_t ssp_obs_dirs_count(void);

int ssp_obs_init(observer obs);
int ssp_obs_destruct(const char* dir_path);

int ssp_obs_process(void);
int ssp_obs_images_list(char **images);

#ifdef __cplusplus
}
#endif
