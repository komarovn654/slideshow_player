#ifndef shader_h
#define shader_h

#include <stdio.h>
#include <GLES2/gl2.h>

typedef struct {
    GLuint type;
    const char* path;
} shader_meta;

int shader_create_program(shader_meta* shaders, int shaders_count);
GLuint shader_get_program(void);
void shader_use_program(void);

#endif /* shader_h */
