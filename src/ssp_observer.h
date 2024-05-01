#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "ssp_helper.h"
#include "ssp_list.h"

#define SSP_OBS_DIRS_COUNT (1)

int ssp_obs_init(const char* dir_path);
int ssp_obs_process(void);
int ssp_obs_images_list(char **images);
ssp_list ssp_obs_images(void);

#ifdef __cplusplus
}
#endif
