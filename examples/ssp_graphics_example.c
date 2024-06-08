#include "logman/logman.h"

#include "ssp_helper.h"
#include "ssp_test_storage.h"
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

int main(int argc, char* argv[])
{
    if (ssp_logman_init() != LOGERR_NOERR) {
        return EXIT_FAILURE;
    }

    ssp_image_storage* is = ssp_test_storage_init_is();
    // is->insert(is->storage, "/Users/nikolajkomarov/dev/slideshow_player/tests/images/rainbow.jpg");
    // is->insert(is->storage, "/Users/nikolajkomarov/dev/slideshow_player/tests/images/bull.jpg");
    is->insert(is->storage, "/Users/nikolajkomarov/dev/slideshow_player/tests/images/church.jpg");
    // is->insert(is->storage, "/Users/nikolajkomarov/dev/slideshow_player/tests/images/girl.jpg");
    // is->insert(is->storage, "/Users/nikolajkomarov/dev/slideshow_player/tests/images/pirate.jpg");
    // is->insert(is->storage, "/Users/nikolajkomarov/dev/slideshow_player/tests/images/shyness.jpg");
    // is->insert(is->storage, "/Users/nikolajkomarov/dev/slideshow_player/tests/images/sins.jpg");
    // is->insert(is->storage, "/Users/nikolajkomarov/dev/slideshow_player/tests/images/wall.jpg");

    if (ssp_window_init(MAX_WINDOW_WIDTH, MAX_WINDOW_HEIGHT, 1.0, is) != 0) {
        return EXIT_FAILURE;
    }

    while (ssp_window_player_loop() == 0) {
    }

    return 0;
}