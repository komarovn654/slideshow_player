#include <stdio.h>
#include <syslog.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "ssp_helper.h"
#include "ssp_image_loader.h"
#include "ssp_memory.h"

ssp_image* ssp_il_read_image(const char* image_path)
{
    ssp_image* image = (ssp_image*)ssp_calloc(1, sizeof(ssp_image));
    image->path = image_path;

    int nrChannels;
    image->data = stbi_load(image->path, &image->width, &image->height, &nrChannels, 0);
    if (!image->data) {
        syslog(LOG_ERR, "SSP STB. Failed to load an image: %s", image_path);
        ssp_free(image);
        return NULL;
    }
    image->buf_size = image->width * image->height * nrChannels;

    syslog(LOG_DEBUG, "SSP STB. STB loaded image <%s>", image->path);
    syslog(LOG_DEBUG, "SSP STB.STB read image params: width: %i, height: %i, size: %i",
        image->width, image->height, image->buf_size);
    return image;
}

void ssp_il_delete_image(ssp_image *image)
{
    stbi_image_free(image->data);
    ssp_free(image);
}
