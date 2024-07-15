#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

typedef struct ssp_image_t {
    int buf_size;
    int width, height;
    int resized_width, resized_height;
    int nr_channels;
    const char* path;
    unsigned char* data;
    unsigned char* resized_data;
    bool need_to_resize;
} ssp_image;

void ssp_il_delete_image(ssp_image* image);
ssp_image* ssp_il_read_image(const char* image_path);

#ifdef __cplusplus
}
#endif
