#include <stdlib.h>
#include <string.h>

#include "ssp_memory.h"
#include "ssp_helper.h"
#include "ssp_test_storage.h"

ssp_static void* ssp_test_storage_insert(void* storage, const char* item_name)
{
    for (size_t i = 0; i < SSP_TS_MAX_ITEM_COUNT; i++) {
        if (strcmp(((char**)storage)[i], "") == 0) {
            size_t len = snprintf(((char**)storage)[i], SSP_TS_ITEM_LEN, "%s", item_name);
            if (len >= SSP_TS_ITEM_LEN) {
                memset(((char**)storage)[i], 0, SSP_TS_ITEM_LEN);
                return NULL;
            }
            return ((char**)storage)[i];
        }
    }
    return NULL;
}

ssp_static void ssp_test_storage_remove(void** storage, const char* item_name)
{
    for (size_t i = 0; i < SSP_TS_MAX_ITEM_COUNT; i++) {
        char* s = ((char**)storage)[i];
        if (strcmp(((char**)storage)[i], item_name) == 0) {
            memset(((char**)storage)[i], 0, SSP_TS_ITEM_LEN);
            return;
        }
    }
    return;
}

ssp_static char* ssp_test_storage_item_name(void* storage)
{
    return ((char**)storage)[0];
}

ssp_static void* ssp_test_storage_move_ptr(void* storage)
{
    return ((char**)storage)[1];
}

ssp_image_storage* ssp_test_storage_init(void)
{
    ssp_image_storage* storage = ssp_malloc(sizeof(ssp_image_storage));
     if (storage == NULL) {
        return NULL;
    }   

    storage->image_name = ssp_test_storage_item_name;
    storage->move_ptr_to_next = ssp_test_storage_move_ptr;
    storage->insert = ssp_test_storage_insert;
    storage->remove = ssp_test_storage_remove;

    storage->storage_head = (char**)ssp_malloc(SSP_TS_MAX_ITEM_COUNT * sizeof(char*));
    if (storage->storage_head == NULL) {
        ssp_free(storage);
        return NULL;
    }
    storage->storage_ptr = storage->storage_head;

    size_t i = 0;
    for (i = 0; i < SSP_TS_MAX_ITEM_COUNT; i++) {
        ((char**)storage->storage_head)[i] = (char*)ssp_calloc(SSP_TS_ITEM_LEN, sizeof(char));
        if (((char**)storage->storage_head)[i] == NULL) {
            for (i = i - 1; i >= 0; i--) {
                ssp_free(((char**)storage->storage_head)[i]);
            }
            ssp_free(storage->storage_head);
            ssp_free(storage);
            return NULL;
        }
    }

    return storage;
}

void ssp_test_storage_destruct(ssp_image_storage* storage)
{
    for (size_t i = 0; i < SSP_TS_MAX_ITEM_COUNT; i++) {
        ssp_free(((char**)storage->storage_head)[i]);
    }

    ssp_free(storage->storage_head);
    ssp_free(storage);
}
