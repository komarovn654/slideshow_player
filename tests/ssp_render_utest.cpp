#include <gtest/gtest.h>

#include "ssp_render.h"
#include "ssp_glfw.h"
#include "ssp_glfw_mock.h"
#include "ssp_glfw_set.h"
#include "ssp_gl.h"
#include "ssp_gl_mock.h"
#include "ssp_gl_set.h"

extern "C"
{
    ssp_static int ssp_render_setup_texture(ssp_render* render);
}

class TestRender : public ::testing::Test {
public:
    void SetUp()
    {
    }
    void TearDown()
    {
        ssp_set_glfw_fptr_default();
        ssp_set_gl_fptr_default();
    }
};

TEST_F(TestRender, SetupTexture)
{
    ssp_gl_set_gen_textures(ssp_gl_gen_textures_mock_p);
    ssp_gl_set_bind_texture(ssp_gl_bind_texture_mock_p);
    ssp_gl_set_tex_parammetri(ssp_gl_tex_parammetri_mock_p);
    ssp_gl_set_bind_texture(ssp_gl_bind_texture_mock_p);

    ssp_render render;
    std:memset(&render, 0, sizeof(ssp_render));
    EXPECT_EQ(ssp_render_setup_texture(&render), 0);
    EXPECT_EQ(render.texture, 1);
}

TEST_F(TestRender, SetupTexture_NULL)
{
    EXPECT_EQ(ssp_render_setup_texture(NULL), 1);
}