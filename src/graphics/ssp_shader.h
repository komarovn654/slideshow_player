#ifndef shader_h
#define shader_h

#include <stdio.h>

#include "ssp_gl.h"

typedef struct ssp_shader_info_t{
    GLuint type;
    // TODO: select from file/const char*
    const char* path;
    char* data;
} ssp_shader_info;

int ssp_shader_create_program(ssp_shader_info* shaders, int shaders_count);
GLuint ssp_shader_get_program(void);
void ssp_shader_use_program(void);

#endif /* shader_h */
