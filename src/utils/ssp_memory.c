#include <stdlib.h>
#include <stdbool.h>

#include "ssp_helper.h"
#include "ssp_memory.h"

static void *ptr_storage[MAX_PTR_STORAGE_LEN] = { NULL };
static size_t size = 0;

void **ssp_ptr_storage()
{
    return ptr_storage;
}

size_t ssp_ptr_storage_size()
{
    return size;
}

size_t ssp_ptr_storage_max_size()
{
    return (size_t)MAX_PTR_STORAGE_LEN;
}

void ssp_reset_ptr_storage_size()
{
    size = 0;
}

static int store_ptr(void *new_ptr)
{
    if (size < MAX_PTR_STORAGE_LEN) {
        ptr_storage[size] = new_ptr;
        size++;
        return 0;
    }

    return 1;
}

static void delete_ptr(void *ptr)
{
    size_t i = 0;
    for (; i < size; i++) {
        if (ptr_storage[i] == ptr) {
            break;
        }
    }

    if (i == size) {
        return;
    }

    for (; i < size - 1; i++) {
        ptr_storage[i] = ptr_storage[i+1];
    }

    ptr_storage[i] = NULL;
    size -= 1;
}

void *ssp_malloc(size_t size)
{
    void *new_ptr = malloc(size);
    if (store_ptr(new_ptr) == 1) {
        ssp_syslog(LOG_CRIT, "SSP. Memory storage overflowed");
        return NULL;
    };

    return new_ptr;
}

void *ssp_calloc(size_t num, size_t size)
{
    void *new_ptr = calloc(num, size);
    if (store_ptr(new_ptr) == 1) {
        ssp_syslog(LOG_CRIT, "SSP. Memory storage overflowed");
        return NULL;
    };

    return new_ptr;
}

void ssp_free(void *ptr)
{
    free(ptr);
    delete_ptr(ptr);
}
