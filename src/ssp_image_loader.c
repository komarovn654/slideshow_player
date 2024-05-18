#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <string.h>
#include <poll.h>
#include <IL/il.h>
#include <unistd.h>

#include "logman/logman.h"

#include "ssp_image_loader.h"
#include "ssp_memory.h"

int ssp_il_init(void)
{
    log_debug("DevIL runtime  version: %i", IL_VERSION);
    log_debug("DevIL compiled version: %i", ilGetInteger(IL_VERSION_NUM));
    if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION) {
        log_error("DevIL version is different!");
        return 1;
    } 

    ilInit();
    return 0;
}

ssp_image* ssp_il_read_image(const char* image_path)
{
    ssp_image* image = (ssp_image*)ssp_calloc(1, sizeof(ssp_image));
    image->path = image_path;

    ilGenImages(1, &image->devil_name);
    ilBindImage(image->devil_name);

    if (ilLoadImage(image->path) == IL_FALSE) {
        log_warning("Devil couldn't load image: %s", image->path);
        ssp_il_delete_image(image);
        return NULL;
    }

    image->width = ilGetInteger(IL_IMAGE_WIDTH);
    image->height = ilGetInteger(IL_IMAGE_HEIGHT); 
    image->data = ilGetData();

    log_debug("Devil loaded image <%s>", image->path);
    log_debug("Devil read image params: width: %i, height: %i, size: %i", image->width, image->height,
        ilGetInteger(IL_IMAGE_SIZE_OF_DATA));
    return image;
}

void ssp_il_delete_image(ssp_image *image)
{
    ilDeleteImages(1, &image->devil_name);
    ssp_free(image);
}
