#include <stdlib.h>

#include "ssp_memory.h"
#include "ssp_helper.h"
#include "ssp_test_storage.h"

// static char* storage[MAX_ITEM_LEN];
// static size_t storage_index;

static void* ssp_test_storage_insert(void* storage, const char* item_name)
{
    // char* s = ((char**)storage)[storage_index];
    // if (strlen(item_name) >= MAX_ITEM_LEN) {
    //     log_warning("Item %s is too big and wasn't insert", item_name);
    //     return NULL;
    // }
    // snprintf(s, MAX_ITEM_LEN, "%s", item_name);
    // storage_index++;
    // log_info("File %s was inserted", item_name);

    return NULL;
}

static void ssp_test_storage_remove(void** storage, const char* item_name)
{
    // char* stripped_item_name = (__builtin_strrchr(item_name, '/') ? __builtin_strrchr(item_name, '/') + 1 : (char*)item_name);

    // for (size_t i = 0; i < storage_index; i++) {
    //     char* s = ((char**)vstorage)[i];
    //     char* stripped_storage_name = (__builtin_strrchr(s, '/') ? __builtin_strrchr(s, '/') + 1 : s);
    //     if (strstr(stripped_item_name, stripped_storage_name) != NULL) {
    //         memset(s, 0, MAX_ITEM_LEN);
    //         log_info("File %s was removed", item_name);
    //         return;
    //     }
    // }

    return;
}

static char* ssp_test_storage_item_name(void* storage)
{
    // char* stripped_item_name = (__builtin_strrchr(item_name, '/') ? __builtin_strrchr(item_name, '/') + 1 : (char*)item_name);

    // for (size_t i = 0; i < storage_index; i++) {
    //     char* s = ((char**)vstorage)[i];
    //     char* stripped_storage_name = (__builtin_strrchr(s, '/') ? __builtin_strrchr(s, '/') + 1 : s);
    //     if (strstr(stripped_item_name, stripped_storage_name) != NULL) {
    //         memset(s, 0, MAX_ITEM_LEN);
    //         log_info("File %s was removed", item_name);
    //         return;
    //     }
    // }

    return NULL;
}

static void* ssp_test_storage_move_ptr(void* storage)
{
    // char* stripped_item_name = (__builtin_strrchr(item_name, '/') ? __builtin_strrchr(item_name, '/') + 1 : (char*)item_name);

    // for (size_t i = 0; i < storage_index; i++) {
    //     char* s = ((char**)vstorage)[i];
    //     char* stripped_storage_name = (__builtin_strrchr(s, '/') ? __builtin_strrchr(s, '/') + 1 : s);
    //     if (strstr(stripped_item_name, stripped_storage_name) != NULL) {
    //         memset(s, 0, MAX_ITEM_LEN);
    //         log_info("File %s was removed", item_name);
    //         return;
    //     }
    // }

    return NULL;
}

ssp_image_storage* ssp_test_storage_init(size_t items_count)
{
    // TODO: free if NULL
    ssp_image_storage* storage = ssp_malloc(sizeof(ssp_image_storage));
     if (storage == NULL) {
        return NULL;
    }   

    storage->image_name = ssp_test_storage_item_name;
    storage->move_ptr_to_next = ssp_test_storage_move_ptr;
    storage->insert = ssp_test_storage_insert;
    storage->remove = ssp_test_storage_remove;

    storage->storage_head = (char**)ssp_malloc(items_count * SSP_TEST_STORAGE_ITEM_LEN);
    if (storage->storage_head == NULL) {
        return NULL;
    }
    storage->storage_ptr = storage->storage_head;

    for (size_t i = 0; i < items_count; i++) {
        ((char**)storage->storage_head)[i] = (char*)ssp_malloc(SSP_TEST_STORAGE_ITEM_LEN);
        if (((char**)storage->storage_head)[i] == NULL) {
            return NULL;
        }
    }

    return storage;
}
