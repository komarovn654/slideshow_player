#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <string.h>
#include <poll.h>
#include <unistd.h>

#include "logman/logman.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "ssp_helper.h"
#include "ssp_image_loader.h"
#include "ssp_memory.h"

ssp_static size_t ssp_il_get_file_size(const char *image_path)
{
    FILE *file = fopen(image_path, "rb");
    if (!file) {
        log_error("Couldn't open the file to find out the size: %s", image_path);
        return -1;
    }

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fclose(file);

    return size;
}

ssp_image* ssp_il_read_image(const char* image_path)
{
    ssp_image* image = (ssp_image*)ssp_calloc(1, sizeof(ssp_image));
    image->path = image_path;

    int nrChannels;
    image->data = stbi_load(image->path, &image->width, &image->height, &nrChannels, 0);
    if (!image->data) {
        log_error("STB. Failed to load image\n");
        ssp_free(image);
        return NULL;
    }

    log_debug("STB loaded image <%s>", image->path);
    log_debug("STB read image params: width: %i, height: %i, size: %i", image->width, image->height, 
        ssp_il_get_file_size(image->path));
    return image;
}

void ssp_il_delete_image(ssp_image *image)
{
    stbi_image_free(image->data);
    ssp_free(image);
}
