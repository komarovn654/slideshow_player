#pragma once
#include <stdio.h>

int ssp_image_loader_init(void);
int read_jpeg(const char* path, unsigned char** buf, int buf_size);
