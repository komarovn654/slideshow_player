#include "glad/glad.h"
#include "GLFW/glfw3.h"

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
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void ssp_render_init_buffers(ssp_render* render)
{
    glGenVertexArrays(1, &(render->buffers.vao_id));
    glBindVertexArray(render->buffers.vao_id);

    glGenBuffers(1, &(render->buffers.vbo_id));
    glBindBuffer(GL_ARRAY_BUFFER, render->buffers.vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(render->vertices), render->vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT,GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void ssp_render_set_shaders(ssp_render* render)
{
    render->shaders[0].type = GL_VERTEX_SHADER;
    render->shaders[0].data = vertex_shader;

    render->shaders[1].type = GL_FRAGMENT_SHADER;
    render->shaders[1].data = fragment_shader;
}

int ssp_render_init_glad(void)
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        ssp_syslog(LOG_ERR, "SSP. GLAD initialization error");
        return 1;
    }

    ssp_syslog(LOG_INFO, "SSP. GLAD was initialized for OpenGL33");
    return 0;
}
