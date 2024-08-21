#include "ssp_gl.h"
#include "ssp_glfw.h"

#include "ssp_helper.h"
#include "ssp_render.h"
#include "ssp_image_loader.h"

char* fragment_shader = "\
#version 330 core\n\
in vec2 TexCoord;\n\
out vec4 color;\n\
uniform sampler2D ourTexture;\n\
void main()\n\
{\n\
	color = texture(ourTexture, TexCoord);\n\
}";

char* vertex_shader = "\
#version 330 core\n\
layout (location = 0) in vec3 position;\n\
layout (location = 1) in vec2 texCoord;\n\
out vec2 TexCoord;\n\
void main()\n\
{\n\
    gl_Position = vec4(position, 1.0f);\n\
    TexCoord = -texCoord;\n\
}";

void ssp_render_set_gl_ctx(void)
{
    ssp_glfw_window_hint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    ssp_glfw_window_hint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    ssp_glfw_window_hint(GLFW_CONTEXT_VERSION_MINOR, 3);
    ssp_glfw_window_hint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    ssp_glfw_window_hint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

int ssp_render_init_buffers(ssp_render render)
{
    if (render == NULL) {
        return 1;
    }

    ssp_gl_gen_vertex_arrays(1, &(render->buffers.vao_id));
    ssp_gl_bind_vertex_array(render->buffers.vao_id);

    ssp_gl_gen_buffers(1, &(render->buffers.vbo_id));
    ssp_gl_bind_buffer(GL_ARRAY_BUFFER, render->buffers.vbo_id);
    ssp_gl_buffer_data(GL_ARRAY_BUFFER, sizeof(render->vertices), render->vertices, GL_STATIC_DRAW);

    // Position attribute
    ssp_gl_vertex_attrib_pointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    ssp_gl_enable_vertex_attrib_array(0);
    // Texture attribute
    ssp_gl_vertex_attrib_pointer(1, 2, GL_FLOAT,GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    ssp_gl_enable_vertex_attrib_array(1);

    ssp_gl_bind_vertex_array(0);
    
    return 0;
}

int ssp_render_set_shaders(ssp_render* render)
{
    if (render == NULL) {
        return 1;
    }

    render->shaders[0].type = GL_VERTEX_SHADER;
    render->shaders[0].data = vertex_shader;

    render->shaders[1].type = GL_FRAGMENT_SHADER;
    render->shaders[1].data = fragment_shader;

    return 0;
}

int ssp_render_init_glad(void)
{
    if (!ssp_glad_load_gl_loader((GLADloadproc)ssp_glfw_get_proc_address)) {
        ssp_syslog(LOG_ERR, "SSP. GLAD initialization error");
        return 1;
    }

    ssp_syslog(LOG_INFO, "SSP. GLAD was initialized for OpenGL33");
    return 0;
}
