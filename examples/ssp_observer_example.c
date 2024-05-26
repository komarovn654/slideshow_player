#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "logman/logman.h"
#include "ssp_observer_ps.h"

#define MAX_NUM_ITEMS (50)
#define MAX_ITEM_LEN  (128)

static char* storage[MAX_ITEM_LEN];
static size_t storage_index;

static void* storage_insert(void* storage, const char* item_name)
{
    char* s = ((char**)storage)[storage_index];
    if (strlen(item_name) >= MAX_ITEM_LEN) {
        log_warning("Item %s is too big and wasn't insert", item_name);
        return NULL;
    }
    snprintf(s, MAX_ITEM_LEN, "%s", item_name);
    storage_index++;
    log_info("File %s was inserted", item_name);

    return NULL;
}

static void  storage_remove(void** vstorage, const char* item_name)
{
    char* stripped_item_name = (__builtin_strrchr(item_name, '/') ? __builtin_strrchr(item_name, '/') + 1 : (char*)item_name);

    for (size_t i = 0; i < storage_index; i++) {
        char* s = ((char**)vstorage)[i];
        char* stripped_storage_name = (__builtin_strrchr(s, '/') ? __builtin_strrchr(s, '/') + 1 : s);
        if (strstr(stripped_item_name, stripped_storage_name) != NULL) {
            memset(s, 0, MAX_ITEM_LEN);
            log_info("File %s was removed", item_name);
            return;
        }
    }

    return;
}

static bool filter_function(const char* item_name)
{
    const char txt_file[] = ".txt";

    if (strstr(item_name, txt_file) != NULL) {
        log_info("File %s passed the filter", item_name);
        return true;
    }

    log_info("File %s was filtered", item_name);
    return false;
}

static int ssp_observer_init(char* dirs[], size_t dirs_count)
{
    ssp_observer obs = {
        .dirs_count = dirs_count,
        .filter = filter_function,
        .storage = storage,
        .storage_insert = storage_insert,
        .storage_remove = storage_remove,
    };
    for (size_t i = 0; i < dirs_count; i++) {
        obs.dirs[i] = (char*)malloc(MAX_ITEM_LEN);
        snprintf(obs.dirs[i], MAX_ITEM_LEN, "%s", dirs[i+2]);
    }

    int result = ssp_obsps_init(obs);

    for (size_t i = 0; i < dirs_count; i++) {
        free(obs.dirs[i]);
    }
    return result;
}

int main(int argc, char *argv[])
{
    int result = EXIT_SUCCESS;

    if (log_init_default() != LOGERR_NOERR) {
        printf("Logger initialization error\n");
        return EXIT_FAILURE;
    }

    if (argc < 3) {
        log_error("usage: ssp_observer_mtest <dirs_count> <dir1_to_observ> <dir2_to_observ> ...up to 10 dirs");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < MAX_NUM_ITEMS; i++) {
        storage[i] = malloc(MAX_ITEM_LEN);
        memset(storage[i], 0, MAX_ITEM_LEN);
    }

    if (ssp_observer_init(argv, (size_t)argv[1][0] - (size_t)'0') != 0) {
        log_error("Observer initialization error");
        return EXIT_FAILURE;
    };

    while(1) {
        if (ssp_obsps_process() != 0) {
            log_error("Observer crashed");
            return EXIT_FAILURE;
        };
    }

    return EXIT_SUCCESS;
}