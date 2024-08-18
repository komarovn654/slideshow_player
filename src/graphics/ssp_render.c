#include "ssp_gl.h"
#include "ssp_glfw.h"

#include "ssp_helper.h"
#include "ssp_render.h"
#include "ssp_image_loader.h"

static ssp_render render = {
    .vertices = SSP_RENDER_TEXTURE_VERTICES,
};

ssp_static int ssp_render_setup_texture(ssp_render* render)
{
    if (render == NULL) {
        return 1;
    }

    ssp_gl_gen_textures(1, &(render->texture));
    ssp_gl_bind_texture(GL_TEXTURE_2D, render->texture);   
    ssp_gl_tex_parammetri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    ssp_gl_tex_parammetri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    ssp_gl_tex_parammetri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  // NOTE the GL_NEAREST Here!
    ssp_gl_tex_parammetri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  // NOTE the GL_NEAREST Here!    
    ssp_gl_bind_texture(GL_TEXTURE_2D, 0);

    return 0;
}

ssp_static int ssp_render_bind_to_texture(const char* image_path, int* width, int* height)
{
    ssp_gl_active_texture(GL_TEXTURE0);
    ssp_gl_bind_texture(GL_TEXTURE_2D, render.texture);

    ssp_image* image = ssp_il_read_image(image_path);
    if (image == NULL) {
        ssp_syslog(LOG_ERR, "SSP. Render couldn't load the image <%s>", image_path);
        return 1;
    }
    
    *width = image->width;
    *height = image->height;
    ssp_gl_tex_image2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->data);
    ssp_gl_generate_mipmap(GL_TEXTURE_2D);
    ssp_gl_bind_texture(GL_TEXTURE_2D, 0);

    ssp_il_delete_image(image);

    return 0;
}

int ssp_render_init(void (*resize_handler)(int width, int height))
{
    ssp_render_init_glad();
    if (ssp_render_init_buffers(&render) != 0) {
        ssp_syslog(LOG_ERR, "SSP. SSP render couldn't init buffers");
        return 1;
    }
    if (ssp_render_set_shaders(&render) != 0) {
        ssp_syslog(LOG_ERR, "SSP. SSP render couldn't set shaders");
        return 2;        
    };
    render.resize_handler = resize_handler;

    if (ssp_shader_create_program(render.shaders, 2) == 0) {
        ssp_syslog(LOG_ERR, "SSP. SSP render couldn't create shader program");
        return 3;
    }

    if (ssp_render_setup_texture(&render) != 0) {
        ssp_syslog(LOG_ERR, "SSP. SSP render couldn't setup texture");
        return 4;
    }

    ssp_syslog(LOG_INFO, "SSP. The render was initialized");
    return 0;
}

ssp_static void ssp_render_draw_error(void)
{
    ssp_gl_clear_color(0.2f, 0.3f, 0.3f, 1.0f);
    ssp_gl_clear(GL_COLOR_BUFFER_BIT);
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
    
    ssp_gl_clear_color(0.2f, 0.3f, 0.3f, 1.0f);
    ssp_gl_clear(GL_COLOR_BUFFER_BIT);

    ssp_gl_active_texture(GL_TEXTURE0);
    ssp_gl_bind_texture(GL_TEXTURE_2D, render.texture);

    ssp_gl_bind_vertex_array(render.buffers.vao_id);

    ssp_gl_draw_arrays(GL_TRIANGLES, 0, 6);
    ssp_gl_bind_buffer(GL_ARRAY_BUFFER, 0);

    return 0;
}
