#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "ssp_helper.h"
#include "ssp_render.h"

const char* fragment_shader = {"\
#version 100\n\
precision mediump float;\n\
vec2 TextCoord;\n\
\n\
vec4 color;\n\
\n\
uniform sampler2D ourTexture;\n\
\n\
void main()\n\
{\n\
	color = vec4(1, 1, 1, 1);\n\
}\n"};

void ssp_render_set_gl_ctx(void)
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

int ssp_render_init_buffers(ssp_render_t* render)
{
    glGenBuffers(1, &(render->buffers.vbo_id));
    glBindBuffer(GL_ARRAY_BUFFER, render->buffers.vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(render->vertices), render->vertices, GL_STATIC_DRAW);
    
    // Position attribute
    GLint mPosition= glGetAttribLocation(ssp_shader_get_program(), "position");
    glVertexAttribPointer(mPosition, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(mPosition);
    // Texture attribute
    GLint mTextCoord= glGetAttribLocation(ssp_shader_get_program(), "textCoord");
    glVertexAttribPointer(mTextCoord, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(mTextCoord);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

int ssp_render_set_shaders(ssp_render_t* render)
{
    render->shaders[0].type = GL_VERTEX_SHADER;
    render->shaders[0].path = "../../src/graphics/shader/gles_vertex.glsl";

    render->shaders[1].type = GL_FRAGMENT_SHADER;
    render->shaders[1].path = "../../src/graphics/shader/gles_fragment.glsl";
}

int ssp_render_init_glad(void)
{
    if (!gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress)) {
        log_error("GLAD initialization error");
        return 1;
    }

    return 0;
}
