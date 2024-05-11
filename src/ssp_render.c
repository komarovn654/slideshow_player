
#include <assert.h>
#include <GLES2/gl2.h>
#include <string.h>

#include "logman/logman.h"

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
        .path = "../../src/shader/vertex.glsl",
    },
    {
        .type = GL_FRAGMENT_SHADER,
        .path = "../../src/shader/fragment.glsl",
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

static int ssp_texture(const char *image_path)
{
    /*
    static int i = 0;
    char *images[10] = {
        "../../tests/images/photo_2021-08-13_16-05-04.jpg",
        "../../tests/images/photo_2023-08-20_15-13-23.jpg",
        "../../tests/images/photo_2023-10-17_16-40-33.jpg",
        "../../tests/images/photo_2023-11-16_10-50-51.jpg",
        "../../tests/images/photo_2024-02-17_21-42-42.jpg"
    };
    int weight[5] = {720, 960, 576, 576, 960};
    int height[5] = {1280, 1280, 1280, 1280, 1280};
    */
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, egl_context.texture);

    ssp_image image = {
        .path = image_path,
    };
    // unsigned char* buf = (unsigned char*)malloc(weight[i]*height[i]*4);
    if (ssp_read_image(&image) != 0) {
        return 1;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    ssp_delete_image(&image);
    /*
    i++;
    if (i >= 5) {
        i = 0;
    }
    */
    return 0;
}

int ssp_render_init()
{
    setup_buffers();

    if (shader_create_program(shaders, 2) == 0) {
        log_error("shader error");
        return 1;
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

static void ssp_draw_error(void)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void ssp_redraw(const char *image)
{ 
    shader_use_program();
    if (ssp_texture(image) != 0) {
        ssp_draw_error();
        return;
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, egl_context.texture);

    glBindBuffer(GL_ARRAY_BUFFER, buffers.vbo_id);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
