#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "logman/logman.h"

#include "ssp_helper.h"
#include "ssp_render.h"
#include "ssp_image_loader.h"

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
    render->shaders[0].path = "../../src/graphics/shader/gl33_vertex.glsl";

    render->shaders[1].type = GL_FRAGMENT_SHADER;
    render->shaders[1].path = "../../src/graphics/shader/gl33_fragment.glsl";
}
