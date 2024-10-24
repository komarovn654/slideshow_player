#include <gtest/gtest.h>

#include "ssp_render.h"
#include "ssp_glfw.h"
#include "ssp_glfw_mock.h"
#include "ssp_glfw_set.h"
#include "ssp_gl.h"
#include "ssp_gl_mock.h"
#include "ssp_gl_set.h"

extern ssp_glfw_hints_t ssp_glfw_hints;

class TestRenderGl33 : public ::testing::Test {
public:
    void SetUp()
    {
        std::memset(&ssp_glfw_hints, 0, sizeof(ssp_glfw_hints_t));
    }
    void TearDown()
    {
        ssp_set_glfw_fptr_default();
        ssp_set_gl_fptr_default();
    }
};

TEST_F(TestRenderGl33, SetGlCtx_OpenGL33)
{
    ssp_glfw_set_window_hint(ssp_glfw_window_hint_mock_p);
    ssp_render_set_gl_ctx();

    EXPECT_EQ(ssp_glfw_hints.client_api, GLFW_OPENGL_API);
    EXPECT_EQ(ssp_glfw_hints.context_version_major, 3);
    EXPECT_EQ(ssp_glfw_hints.context_version_minor, 3);
    EXPECT_EQ(ssp_glfw_hints.opengl_forward_compact, GL_TRUE);
    EXPECT_EQ(ssp_glfw_hints.opengl_profile, GLFW_OPENGL_CORE_PROFILE);
}

TEST_F(TestRenderGl33, InitBuffers_NULL)
{
    EXPECT_EQ(ssp_render_init_buffers(NULL), 1);
}

TEST_F(TestRenderGl33, InitBuffers)
{
    ssp_render_t render;
    std::memset(&render, 0, sizeof(ssp_render_t));

    ssp_gl_set_gen_vertex_arrays(ssp_gl_gen_vertex_arrays_mock_p);
    ssp_gl_set_bind_vertex_array(ssp_gl_bind_vertex_array_mock_p);
    ssp_gl_set_gen_buffers(ssp_gl_gen_buffers_mock_p);
    ssp_gl_set_bind_buffer(ssp_gl_bind_buffer_mock_p);
    ssp_gl_set_buffer_data(ssp_gl_buffer_data_mock_p);
    ssp_gl_set_vertex_attrib_pointer(ssp_gl_vertex_attrib_pointer_mock_p);
    ssp_gl_set_enable_vertex_attrib_array(ssp_gl_enable_vertex_attrib_array_mock_p);

    EXPECT_EQ(ssp_render_init_buffers(&render), 0);
    EXPECT_EQ(render.buffers.vao_id, 1);
    EXPECT_EQ(render.buffers.vbo_id, 1);
}

TEST_F(TestRenderGl33, SetShaders_NULL)
{
    EXPECT_EQ(ssp_render_set_shaders(NULL), 1);
}

TEST_F(TestRenderGl33, SetShaders)
{
    ssp_render_t render;
    std::memset(&render, 0, sizeof(ssp_render_t));
    EXPECT_EQ(ssp_render_set_shaders(&render), 0);

    EXPECT_EQ(render.shaders[0].type, GL_VERTEX_SHADER);
    EXPECT_TRUE(render.shaders[0].data != NULL);

    EXPECT_EQ(render.shaders[1].type, GL_FRAGMENT_SHADER);
    EXPECT_TRUE(render.shaders[1].data != NULL);
}

TEST_F(TestRenderGl33, InitGlad_GlLoaderFail)
{
    ssp_glad_set_load_gl_loader((ssp_glad_load_gl_loader_t)ssp_glad_load_gl_loader_mock_f);

    EXPECT_EQ(ssp_render_init_glad(), 1);
}

TEST_F(TestRenderGl33, InitGlad)
{
    ssp_glad_set_load_gl_loader((ssp_glad_load_gl_loader_t)ssp_glad_load_gl_loader_mock_p);

    EXPECT_EQ(ssp_render_init_glad(), 0);
}
