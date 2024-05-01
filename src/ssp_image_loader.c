#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <string.h>
#include <poll.h>
#include <IL/il.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <logman/logman.h>

#include "ssp_image_loader.h"

int ssp_image_loader_init(void)
{
    log_debug("DevIL runtime  version: %i", IL_VERSION);
    log_debug("DevIL compiled version: %i", ilGetInteger(IL_VERSION_NUM));
    if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION) {
        log_error("DevIL version is different...exiting!");
        return 1;
    } 

    ilInit();
    return 0;
}

int ssp_read_image(ssp_image *image)
{
    ilGenImages(1, &image->devil_name);
    ilBindImage(image->devil_name);

    if (ilLoadImage(image->path) == IL_FALSE) {
        log_warning("devil couldn't load image: %s", image->path);
        ssp_delete_image(image);
        return 1;
    }

    image->width = ilGetInteger(IL_IMAGE_WIDTH);
    image->height = ilGetInteger(IL_IMAGE_HEIGHT); 
    image->data = ilGetData();

    log_debug("image %s has been loaded", image->path);
    log_debug("image params: width: %i, height: %i, size: %i", image->width, image->height,
        ilGetInteger(IL_IMAGE_SIZE_OF_DATA));
    return 0;
}

void ssp_delete_image(ssp_image *image)
{
    ilDeleteImages(1, &image->devil_name); 
}
