#pragma once
#include <stdio.h>

#include "IL/il.h"

typedef struct {
    const char* path;
    unsigned int devil_name;
    unsigned char* data;
    unsigned int buf_size, width, height;
} ssp_image;

int ssp_image_loader_init(void);
void ssp_delete_image(ssp_image *image);
int ssp_read_image(ssp_image *image);
void ssp_observe_image_dir(void);
