#pragma once
#include <stdio.h>

#include "IL/il.h"

typedef struct {
    const char* path;
    unsigned int devil_name;
    unsigned char* data;
    unsigned int buf_size, width, height;
} ssp_image;

int ssp_il_init(void);
void ssp_il_delete_image(ssp_image* image);
ssp_image* ssp_il_read_image(const char* image_path);
