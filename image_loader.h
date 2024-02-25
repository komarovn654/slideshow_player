//
//  image_loader.h
//  OpenGL_lessons
//
//  Created by Николай Комаров on 14.01.2024.
//

#ifndef image_loader_h
#define image_loader_h

#include <stdio.h>

int read_jpeg(const char* path, unsigned char** buf, int buf_size);

#endif /* image_loader_h */
