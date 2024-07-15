#include <stdio.h>
#include <syslog.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_resize2.h"
#include "ssp_helper.h"
#include "ssp_image_loader.h"
#include "ssp_memory.h"

ssp_static size_t ssp_il_resize_if_needed(ssp_image* image)
{
    image->resized_width = image->width, image->resized_height = image->height;
    if (image->width % 2 != 0) {
        image->resized_width -= 1;
        image->need_to_resize = true;
    }
    if (image->height % 2 != 0) {
        image->resized_height -= 1;
        image->need_to_resize = true;
    }
    if (image->need_to_resize) {
        if ((image->resized_data = ssp_malloc(image->resized_width * image->resized_height * image->nr_channels)) == NULL) {
            syslog(LOG_ERR, "SSP STB. Failed to allocate memory for image resizing: %s", image->path);
            return 1;            
        }
        stbir_resize_uint8_linear(image->data, image->width, image->height, 0, 
            image->resized_data, image->resized_width, image->resized_height, 0, image->nr_channels);

        stbi_image_free(image->data);    
        image->data = image->resized_data;
        image->width = image->resized_width;
        image->height = image->resized_height;
    }

    return 0;
}

ssp_image* ssp_il_read_image(const char* image_path)
{
    ssp_image* image = (ssp_image*)ssp_calloc(1, sizeof(ssp_image));
    image->need_to_resize = false;
    image->path = image_path;

    image->data = stbi_load(image->path, &image->width, &image->height, &image->nr_channels, 0);
    if (!image->data) {
        syslog(LOG_ERR, "SSP STB. Failed to load an image: %s", image_path);
        ssp_free(image);
        return NULL;
    }

    if (ssp_il_resize_if_needed(image) != 0) {
        ssp_free(image);
        return NULL;
    }

    image->buf_size = image->width * image->height * image->nr_channels;
    syslog(LOG_DEBUG, "SSP STB. STB loaded image <%s>", image->path);
    syslog(LOG_DEBUG, "SSP STB.STB read image params: width: %i, height: %i, size: %i",
        image->width, image->height, image->buf_size);
    return image;
}

void ssp_il_delete_image(ssp_image *image)
{
    if (image->need_to_resize) {
        ssp_free(image->resized_data);
    } else {
        stbi_image_free(image->data);
    }
    ssp_free(image);
}
