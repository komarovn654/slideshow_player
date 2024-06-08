#include <stdlib.h>
#include <string.h>

#include "logman/logman.h"
#include "ssp_memory.h"
#include "ssp_helper.h"
#include "ssp_test_storage.h"

ssp_static void* ssp_test_storage_insert(void* storage, const char* item_name)
{
    ssp_test_storage* ts = (ssp_test_storage*)storage;

    if (ts->len >= ts->cap) {
        return NULL;
    }

    size_t index = ts->len;
    size_t item_len = snprintf(ssp_ts_char_ptr(ts)[index], SSP_FULL_NAME_MAX_LEN, "%s", item_name);
    if (item_len >= SSP_FULL_NAME_MAX_LEN) {
        return NULL;
    }
    log_info("%s has been added", ssp_ts_char_ptr(ts)[index]);
    ts->len++;

    return ssp_ts_char_ptr(ts)[index];
}

ssp_static void ssp_test_storage_remove(void** storage, const char* item_name)
{
    ssp_test_storage** ts = (ssp_test_storage**)storage;

    size_t removed_index = 0;
    for (removed_index = 0; removed_index < (*ts)->len; removed_index++) {
        if (strcmp(ssp_ts_char_ptr(*ts)[removed_index], item_name) == 0) {
            memset(ssp_ts_char_ptr(*ts)[removed_index], 0, SSP_FULL_NAME_MAX_LEN);
            (*ts)->len--;
            break;
        }
    }

    if (removed_index >= (*ts)->len) {
        return;
    }

    for (size_t i = removed_index; i < (*ts)->len; i++) {
        memcpy(ssp_ts_char_ptr(*ts)[i], ssp_ts_char_ptr(*ts)[i+1], SSP_FULL_NAME_MAX_LEN);
    }

    memset(ssp_ts_char_ptr(*ts)[(*ts)->len], 0, SSP_FULL_NAME_MAX_LEN);
    return;
}

ssp_static char* ssp_test_storage_item_name(void* storage)
{
    ssp_test_storage* ts = (ssp_test_storage*)storage;

    return ssp_ts_char_ptr(ts)[ts->position];
}

ssp_static void* ssp_test_storage_move_ptr(void* storage)
{
    ssp_test_storage* ts = (ssp_test_storage*)storage;

    if (ts->position + 1 >= ts->len) {
        ts->position = 0;
        return NULL;
    }

    ts->position++;
    return storage;
}

ssp_static void ssp_test_storage_destruct(ssp_test_storage* storage)
{
    for (size_t i = 0; i < storage->cap; i++) {
        ssp_free(ssp_ts_char_ptr(storage)[i]);
    }
    ssp_free(storage->head_ptr);
    ssp_free(storage);
}

ssp_static ssp_test_storage* ssp_test_storage_init(void)
{
    ssp_test_storage* test_storage = ssp_malloc(sizeof(ssp_test_storage));
    if (test_storage == NULL) {
        return NULL;
    }

    if ((test_storage->head_ptr = (char**)ssp_malloc(SSP_TS_MAX_ITEM_COUNT * sizeof(char*))) == NULL) {
        ssp_free(test_storage);
        return NULL;
    }
    test_storage->len = 0;
    test_storage->cap = 0;
    test_storage->position = 0;

    for (size_t i = 0; i < SSP_TS_MAX_ITEM_COUNT; i++) {
        ssp_ts_char_ptr(test_storage)[i] = (char*)ssp_calloc(SSP_FULL_NAME_MAX_LEN, sizeof(char));
        if (ssp_ts_char_ptr(test_storage)[i] == NULL) {
            ssp_test_storage_destruct(test_storage);
            return NULL;
        }
        test_storage->cap++;
    }

    return test_storage;
}

ssp_image_storage* ssp_test_storage_init_is(void)
{
    ssp_test_storage* test_storage = ssp_test_storage_init();
    if (test_storage == NULL) {
        return NULL;
    }

    ssp_image_storage* storage = ssp_malloc(sizeof(ssp_image_storage));
    if (storage == NULL) {
        ssp_test_storage_destruct(test_storage);
        return NULL;
    }   

    storage->storage = test_storage;
    storage->image_name = ssp_test_storage_item_name;
    storage->move_to_next = ssp_test_storage_move_ptr;
    storage->insert = ssp_test_storage_insert;
    storage->remove = ssp_test_storage_remove;

    return storage;
}

void ssp_test_storage_destruct_is(ssp_image_storage* is)
{
    ssp_test_storage_destruct((ssp_test_storage*)is->storage);
    ssp_free(is);
}
