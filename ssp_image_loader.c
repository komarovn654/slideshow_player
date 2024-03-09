#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <IL/il.h>
#include <logger.h>

#include "ssp_image_loader.h"

int ssp_image_loader_init(void)
{
    log_debug("DevIL runtime  version: %i", IL_VERSION);
    log_debug("DevIL compiled version: %i", ilGetInteger(IL_VERSION_NUM));
    if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION) {
        log_error("DevIL version is different...exiting!");
        return 1;
    } 

    return 0;
}

int read_jpeg(const char* path, unsigned char** buf, int buf_size)
{
    ILuint ImageName;
    ilGenImages(1, &ImageName);
    ilBindImage(ImageName);

    
    ilLoad(IL_TYPE_BIT, path);

    log_info("done");
    if (ilLoadImage(path) == IL_FALSE) {
        log_error("error");
        return 1;
    }
    
    log_info("%s", path);
    ILuint Width, Height;
    Width = ilGetInteger(IL_IMAGE_WIDTH);
    Height = ilGetInteger(IL_IMAGE_HEIGHT); 
    log_debug("width, height %i %i", Width, Height);


    ilDeleteImages(1, &ImageName); 
    return 0;
}