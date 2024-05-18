#include "logman/logman.h"

#include "ssp_helper.h"
#include "ssp_observer_ps.h"
#include "ssp_image_loader.h"
#include "ssp_list.h"
#include "ssp_window.h"

static void logman_error_callback(void)
{
    fprintf(stderr, "%s\n", log_get_internal_error());
}

ssp_static logman_error ssp_logman_init(void)
{
    logman_settings settings = {
        .type = LOGTYPE_DEBUG,
        .out_type = LOGOUT_STREAM,
        .output.out_stream = stderr,
        .error_callback = logman_error_callback
    };

    logman_error err = log_init(&settings);
    if (log_init(&settings) != LOGERR_NOERR) {
        fprintf(stderr, "logman initialization error: %d\n", err);
        return err;
    }

    return LOGERR_NOERR;
}

ssp_static int ssp_observer_init(ssp_list image_storage)
{
    ssp_observer obs_settings = {
        .dirs_count = 1,
        .storage = (void*)image_storage,
        .storage_insert = ssp_list_insertv,
        .storage_remove = ssp_list_removev_node,
        .filter = ssp_is_file_image,
    };
    obs_settings.dirs[0] = "/Users/nikolajkomarov/dev/slideshow_player/tests/images/";

    if (ssp_obsps_init(obs_settings) != 0) {
        return 1;
    }

    return 0;
}

int main(void)
{
    if (ssp_logman_init() != LOGERR_NOERR) {
        return EXIT_FAILURE;
    }

    ssp_list image_list = ssp_list_init();
    if (image_list == NULL) {
        return EXIT_FAILURE;
    }

    if (ssp_observer_init(image_list) != 0) {
        return EXIT_FAILURE;
    }

    ssp_image_storage* image_storage = (ssp_image_storage*)ssp_calloc(1, sizeof(ssp_image_storage));
    image_storage->storage = (void*)image_list;
    image_storage->insert = ssp_list_insertv;
    image_storage->remove = ssp_list_removev_node;
    image_storage->move_ptr_to_next = ssp_list_move_headv;
    image_storage->image_name = ssp_list_head_namev;

    if (ssp_window_init(400, 400, 10.0, image_storage) != 0) {
        return EXIT_FAILURE;
    }

    while (1) {
        ssp_window_player_loop();
    }
}