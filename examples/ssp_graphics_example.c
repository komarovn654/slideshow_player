#include <syslog.h>
#include <stdlib.h>

#include "ssp_helper.h"
#include "ssp_test_storage.h"
#include "ssp_window.h"

int main(int argc, char* argv[])
{
    ssp_image_storage* is = ssp_test_storage_init_is();
    is->insert(is->storage, "/Users/nikolajkomarov/dev/slideshow_player/tests/images/rainbow.jpg");
    is->insert(is->storage, "/Users/nikolajkomarov/dev/slideshow_player/tests/images/bull.jpg");
    is->insert(is->storage, "/Users/nikolajkomarov/dev/slideshow_player/tests/images/church.jpg");
    is->insert(is->storage, "/Users/nikolajkomarov/dev/slideshow_player/tests/images/girl.jpg");
    is->insert(is->storage, "/Users/nikolajkomarov/dev/slideshow_player/tests/images/pirate.jpg");
    is->insert(is->storage, "/Users/nikolajkomarov/dev/slideshow_player/tests/images/shyness.jpg");
    is->insert(is->storage, "/Users/nikolajkomarov/dev/slideshow_player/tests/images/sins.jpg");
    is->insert(is->storage, "/Users/nikolajkomarov/dev/slideshow_player/tests/images/wall.jpg");

    if (ssp_window_init(MAX_WINDOW_WIDTH, MAX_WINDOW_HEIGHT, 5.0, is) != 0) {
        return EXIT_FAILURE;
    }

    while (ssp_window_player_loop() == 0) {
    }

    return 0;
}