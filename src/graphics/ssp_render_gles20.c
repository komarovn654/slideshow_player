#include "ssp_gl.h"
#include "ssp_glfw.h"

#include "ssp_helper.h"
#include "ssp_render.h"
#include "ssp_memory.h"

const char* fragment_shader =
        "precision mediump float;                               \n"
        "varying vec2 v_texCoord;                               \n"
        "uniform sampler2D s_texture;                           \n"
        "void main()                                            \n"
        "{                                                      \n"
        "  gl_FragColor = texture2D( s_texture, v_texCoord );   \n"
        "}                                                      \n";

char* vertex_shader =
        "attribute vec2 a_texCoord;                                     \n"
        "attribute vec4 a_position;                                     \n"
        "varying vec2 v_texCoord;                                       \n"
        "void main()                                                    \n"
        "{                                                              \n"
        "   gl_Position = a_position;                                   \n"
        "   v_texCoord = vec2(1.0 - a_texCoord.x, 1.0 - a_texCoord.y);  \n"
        "}                                                              \n";

void ssp_render_set_gl_ctx(void)
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

int ssp_render_init_buffers(ssp_render_t* render)
{
    if (render == NULL) {
        return 1;
    }

    ssp_gl_gen_buffers(1, &(render->buffers.vbo_id));
    ssp_gl_bind_buffer(GL_ARRAY_BUFFER, render->buffers.vbo_id);
    ssp_gl_buffer_data(GL_ARRAY_BUFFER, sizeof(render->vertices), render->vertices, GL_STATIC_DRAW);
            
    // Position attribute
    ssp_gl_vertex_attrib_pointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    ssp_gl_enable_vertex_attrib_array(0);
    // Texture attribute
    ssp_gl_vertex_attrib_pointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    ssp_gl_enable_vertex_attrib_array(1);

    ssp_gl_bind_buffer(GL_ARRAY_BUFFER, 0);
    return 0;
}

int ssp_render_set_shaders(ssp_render_t* render)
{
    if (render == NULL) {
        return 1;
    }

    render->shaders = ssp_malloc(2 * sizeof(ssp_shader_info));
    if (render->shaders == NULL) {
        return 2;
    }

    render->shaders[0].type = GL_VERTEX_SHADER;
    render->shaders[0].data = vertex_shader;

    render->shaders[1].type = GL_FRAGMENT_SHADER;
    render->shaders[1].data = fragment_shader;

    return 0;
}

int ssp_render_init_glad(void)
{
    if (!ssp_glad_load_gl_loader((GLADloadproc)glfwGetProcAddress)) {
        ssp_syslog(LOG_ERR, "SSP. GLAD ES2 initialization error");
        return 1;
    }

    return 0;
}
