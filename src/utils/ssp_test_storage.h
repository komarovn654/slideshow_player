#ifndef test_storage_h
#define test_storage_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "ssp_helper.h"

#define SSP_TS_MAX_ITEM_COUNT (20)

typedef struct ssp_test_storage_t {
    char** head_ptr;
    size_t len, position, cap;
} ssp_test_storage;

#define ssp_ts_char_ptr(test_storage) ((char**)(test_storage)->head_ptr)
#define ssp_is_char_ptr(image_storage) ((char**)((ssp_test_storage*)(image_storage->storage))->head_ptr)

ssp_image_storage* ssp_test_storage_init_is(void);
void ssp_test_storage_destruct_is(ssp_image_storage* is);

#ifdef __cplusplus
}
#endif

#endif /* helper_h */
