#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

#define MAX_PTR_STORAGE_LEN 1000

void* ssp_malloc(size_t);
void *ssp_calloc(size_t num, size_t size);
void ssp_free(void *);
size_t ssp_ptr_storage_size(void);
size_t ssp_ptr_storage_max_size(void);

#ifdef __cplusplus
}
#endif
