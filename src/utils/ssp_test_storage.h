#ifndef test_storage_h
#define test_storage_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "ssp_helper.h"

#define SSP_TEST_STORAGE_ITEM_LEN (SSP_FILE_NAME_MAX_LEN)

ssp_image_storage* ssp_test_storage_init(size_t items_count);

#ifdef __cplusplus
}
#endif

#endif /* helper_h */
