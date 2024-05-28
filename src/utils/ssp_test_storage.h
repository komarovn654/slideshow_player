#ifndef test_storage_h
#define test_storage_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "ssp_helper.h"

#define SSP_TS_MAX_ITEM_COUNT (20)
#define SSP_TS_ITEM_LEN (SSP_FILE_NAME_MAX_LEN)

ssp_image_storage* ssp_test_storage_init();
void ssp_test_storage_destruct(ssp_image_storage* storage);

#ifdef __cplusplus
}
#endif

#endif /* helper_h */
