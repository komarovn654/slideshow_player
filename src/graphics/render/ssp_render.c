#include "ssp_gl.h"

#include "ssp_helper.h"
#include "ssp_render.h"
#include "ssp_image_loader.h"

#define SSP_RENDER_TEXTURE_VERTICES {  \
    -1.0f,  1.0f, 0.0f,     1.0f, 1.0f,\
    -1.0f, -1.0f, 0.0f,     1.0f, 0.0f,\
     1.0f, -1.0f, 0.0f,     0.0f, 0.0f,\
     1.0f, -1.0f, 0.0f,     0.0f, 0.0f,\
     1.0f,  1.0f, 0.0f,     0.0f, 1.0f,\
    -1.0f,  1.0f, 0.0f,     1.0f, 1.0f,\
}

typedef struct ssp_render_buffers_t {
    GLuint vao_id; // vertex array object
    GLuint ebo_id; // element buffer object
    GLuint vbo_id; // vertex buffer object
} ssp_render_buffers;

typedef void (*ssp_render_resize_handler_t)(int width, int height);
typedef int (*ssp_render_init_buffers_t)(void);
typedef int (*ssp_render_set_shaders_t)(void);

typedef struct ssp_render_t {
    GLfloat vertices[30];
    ssp_shader_info shaders[2];
    ssp_render_buffers buffers;
    GLuint texture;

    ssp_render_resize_handler_t resize_handler;
    ssp_render_init_buffers_t init_buffers;
    ssp_render_set_shaders_t set_shaders;
} ssp_render_t;

static ssp_render_t ssp_render = {
    .vertices = SSP_RENDER_TEXTURE_VERTICES,
    .init_buffers = ssp_render_init_buffers,
    .set_shaders = ssp_render_set_shaders,
};

void ssp_render_set_fptr_default(void)
{
    ssp_render.init_buffers = ssp_render_init_buffers;
    ssp_render.set_shaders = ssp_render_set_shaders;
}

ssp_static int ssp_render_setup_texture(ssp_render_t* render)
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
    if ((image_path == NULL) || (width == NULL) || (height == NULL)) {
        ssp_syslog(LOG_ERR, "SSP. NULL arg");
        return 1;
    }

    ssp_image* image = ssp_il_read_image(image_path);
    if (image == NULL) {
        ssp_syslog(LOG_ERR, "SSP. Render couldn't load the image <%s>", image_path);
        return 2;
    }
    *width = image->width;
    *height = image->height;

    ssp_gl_active_texture(GL_TEXTURE0);
    ssp_gl_bind_texture(GL_TEXTURE_2D, ssp_render.texture);
    ssp_gl_tex_image2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->data);
    ssp_gl_generate_mipmap(GL_TEXTURE_2D);
    ssp_gl_bind_texture(GL_TEXTURE_2D, 0);

    ssp_il_delete_image(image);

    return 0;
}

int ssp_render_init(ssp_render_resize_handler_t resize_handler)
{
    if (resize_handler == NULL) {
        ssp_syslog(LOG_ERR, "SSP. NULL args");
        return 1;
    }
    ssp_render.resize_handler = resize_handler;

    if (ssp_render_init_glad() != 0) {
        ssp_syslog(LOG_ERR, "SSP. SSP render couldn't init glad");
        return 2;
    };

    if (ssp_render_init_buffers() != 0) {
        ssp_syslog(LOG_ERR, "SSP. SSP render couldn't init buffers");
        return 3;
    }

    if (ssp_render_set_shaders() != 0) {
        ssp_syslog(LOG_ERR, "SSP. SSP render couldn't set shaders");
        return 4;        
    };
    
    if (ssp_shader_create_program(ssp_render.shaders, 2) == 0) {
        ssp_syslog(LOG_ERR, "SSP. SSP render couldn't create shader program");
        return 5;
    }

    if (ssp_render_setup_texture(&ssp_render) != 0) {
        ssp_syslog(LOG_ERR, "SSP. SSP render couldn't setup texture");
        return 6;
    }

    ssp_syslog(LOG_INFO, "SSP. OpenGL version: %s", ssp_gl_get_string(GL_VERSION));
    ssp_syslog(LOG_INFO, "SSP. The render was initialized");
    return 0;
}

void ssp_render_viewport(GLint x_offset, GLint y_offset, GLsizei width, GLsizei height)
{
    ssp_gl_viewport(x_offset, 0, width, height);
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
    ssp_render.resize_handler(width, height);
    
    ssp_gl_clear_color(0.2f, 0.3f, 0.3f, 1.0f);
    ssp_gl_clear(GL_COLOR_BUFFER_BIT);

    ssp_gl_active_texture(GL_TEXTURE0);
    ssp_gl_bind_texture(GL_TEXTURE_2D, render.texture);

    ssp_gl_bind_vertex_array(render.buffers.vao_id);

    ssp_gl_draw_arrays(GL_TRIANGLES, 0, 6);
    ssp_gl_bind_buffer(GL_ARRAY_BUFFER, 0);

    return 0;
}
