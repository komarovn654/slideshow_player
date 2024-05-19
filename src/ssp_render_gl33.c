#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "logman/logman.h"

#include "ssp_helper.h"
#include "ssp_render.h"
#include "ssp_image_loader.h"

static ssp_render render = {
    .vertices = SSP_RENDER_TEXTURE_VERTICES,
    .shaders = {
        {
            .type = GL_VERTEX_SHADER,
            .path = "../../src/shader/gl33_vertex.glsl",
        },
        {
            .type = GL_FRAGMENT_SHADER,
            .path = "../../src/shader/gl33_fragment.glsl",
        },        
    }
};

void ssp_render_set_gl_ctx(void)
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

ssp_static void ssp_render_init_buffers(void)
{
    glGenVertexArrays(1, &render.buffers.vao_id);
    glBindVertexArray(render.buffers.vao_id);

    glGenBuffers(1, &render.buffers.vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, render.buffers.vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(render.vertices), render.vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT,GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

ssp_static int ssp_render_bind_to_texture(const char *image_path)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, render.texture);

    ssp_image* image = ssp_il_read_image(image_path);
    if (image == NULL) {
        log_error("SSP render couldn't load the image <%s>", image_path);
        return 1;
    }
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    ssp_il_delete_image(image);

    return 0;
}

int ssp_render_init(void)
{
    ssp_render_init_buffers();
    
    if (shader_create_program(render.shaders, 2) == 0) {
        log_error("SSP render couldn't create shader program");
        return 1;
    }

    glGenTextures(1, &render.texture);
    glBindTexture(GL_TEXTURE_2D, render.texture);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  // NOTE the GL_NEAREST Here!
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  // NOTE the GL_NEAREST Here!    
    glBindTexture(GL_TEXTURE_2D, 0);

    return 0;
}

ssp_static void ssp_render_draw_error(void)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

int ssp_render_redraw(const char* image)
{ 
    shader_use_program();
    
    if (ssp_render_bind_to_texture(image) != 0) {
        log_error("SSP render couldn't bind image to texture");
        ssp_render_draw_error();
        return 1;
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, render.texture);

    glBindVertexArray(render.buffers.vbo_id);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return 0;
}
