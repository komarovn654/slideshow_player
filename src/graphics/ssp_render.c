#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "ssp_helper.h"
#include "ssp_render.h"
#include "ssp_image_loader.h"

static ssp_render render = {
    .vertices = SSP_RENDER_TEXTURE_VERTICES,
};

ssp_static int ssp_render_bind_to_texture(const char* image_path, int* width, int* height)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, render.texture);

    ssp_image* image = ssp_il_read_image(image_path);
    if (image == NULL) {
        ssp_syslog(LOG_ERR, "SSP. Render couldn't load the image <%s>", image_path);
        return 1;
    }
    
    *width = image->width;
    *height = image->height;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    ssp_il_delete_image(image);

    return 0;
}

int ssp_render_init(void (*resize_handler)(int width, int height))
{
    ssp_render_init_glad();
    ssp_render_init_buffers(&render);
    ssp_render_set_shaders(&render);
    render.resize_handler = resize_handler;

    if (ssp_shader_create_program(render.shaders, 2) == 0) {
        ssp_syslog(LOG_ERR, "SSP. SSP render couldn't create shader program");
        return 1;
    }

    glGenTextures(1, &render.texture);
    glBindTexture(GL_TEXTURE_2D, render.texture);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  // NOTE the GL_NEAREST Here!
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  // NOTE the GL_NEAREST Here!    
    glBindTexture(GL_TEXTURE_2D, 0);

    ssp_syslog(LOG_INFO, "SSP. The render was initialized");
    return 0;
}

ssp_static void ssp_render_draw_error(void)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

int ssp_render_redraw(const char* image)
{ 
    ssp_shader_use_program();
    
    int width, height;
    if (ssp_render_bind_to_texture(image, &width, &height) != 0) {
        ssp_syslog(LOG_ERR, "SSP. The render couldn't bind image <%s> to texture", image);
        ssp_render_draw_error();
        return 1;
    }
    render.resize_handler(width, height);
    
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, render.texture);

    glBindVertexArray(render.buffers.vao_id);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return 0;
}
