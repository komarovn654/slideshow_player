
#include <assert.h>
#include <GLES2/gl2.h>
#include <logger.h>
#include <string.h>

#include "ssp_shader.h"
#include "ssp_image_loader.h"

GLfloat vertices[] = {
    -1.0f,  1.0f, 0.0f,     0.0f, 1.0f, 
    -1.0f, -1.0f, 0.0f,     0.0f, 0.0f,
     1.0f, -1.0f, 0.0f,     1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,     1.0f, 0.0f, 
     1.0f,  1.0f, 0.0f,     1.0f, 1.0f,
    -1.0f,  1.0f, 0.0f,     0.0f, 1.0f,
};  

static shader_meta shaders[2] = {
    {
        .type = GL_VERTEX_SHADER,
        .path = "vertex.glsl",
    },
    {
        .type = GL_FRAGMENT_SHADER,
        .path = "fragment.glsl",
    }
};

static struct {
    GLuint vao_id; // vertex array object
    GLuint ebo_id; // element buffer object
    GLuint vbo_id; // vertex buffer object
} buffers;

static struct t_egl_context {
    struct wl_egl_window *egl_window;

    GLuint texture;
    GLuint texture2;
} egl_context;

static void setup_buffers(void)
{
    glGenBuffers(1, &buffers.vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, buffers.vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

static void ssp_texture()
{
    static int i = 0;
    char *images[5] = {
        "images/photo_2021-08-13_16-05-04.jpg", 
        "images/photo_2023-08-20_15-13-23.jpg",
        "images/photo_2023-10-17_16-40-33.jpg",
        "images/photo_2023-11-16_10-50-51.jpg",
        "images/photo_2024-02-17_21-42-42.jpg"};
    int weight[5] = {720, 960, 576, 576, 960};
    int height[5] = {1280, 1280, 1280, 1280, 1280};

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, egl_context.texture);

    unsigned char* buf = (unsigned char*)malloc(weight[i]*height[i]*3);
    read_jpeg(images[i], &buf, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, weight[i], height[i], 0, GL_RGB, GL_UNSIGNED_BYTE, buf);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    i++;
    if (i >= 5) {
        i = 0;
    }
}

int ssp_render_init()
{
    setup_buffers();

    if (shader_create_program(shaders, 2) == 0) {
        log_error("Shader error");
    }

    glGenTextures(1, &egl_context.texture);
    glBindTexture(GL_TEXTURE_2D, egl_context.texture);   

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  // NOTE the GL_NEAREST Here!
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  // NOTE the GL_NEAREST Here!    

    glBindTexture(GL_TEXTURE_2D, 0);

    return 0;
}

void ssp_redraw()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    shader_use_program();
    ssp_texture();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, egl_context.texture);

    glBindBuffer(GL_ARRAY_BUFFER, buffers.vbo_id);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
