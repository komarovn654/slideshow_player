#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ssp_image_t {
    const char* path;
    unsigned char* data;
    int buf_size, width, height;
} ssp_image;

void ssp_il_delete_image(ssp_image* image);
ssp_image* ssp_il_read_image(const char* image_path);

#ifdef __cplusplus
}
#endif
