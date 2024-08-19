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
    ssp_static int ssp_render_bind_to_texture(const char* image_path, int* width, int* height);
    void ssp_resize_handler_mock(int image_width, int image_height) { return; }
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

TEST_F(TestRender, BindToTexture_NULL)
{
    const char* ip = "path";
    int w, h;
    EXPECT_EQ(ssp_render_bind_to_texture(NULL, &w, &h), 1);
    EXPECT_EQ(ssp_render_bind_to_texture(ip, NULL, &h), 1);
    EXPECT_EQ(ssp_render_bind_to_texture(ip, &w, NULL), 1);
}

TEST_F(TestRender, BindToTexture_ILError)
{
    const char* ip = "invalid_path";
    int w, h;
    EXPECT_EQ(ssp_render_bind_to_texture(ip, &w, &h), 2);
}

TEST_F(TestRender, BindToTexture)
{
    const char* images_path = "../../tests/images/rainbow.jpg";
    int w, h;

    ssp_gl_set_active_texture(ssp_gl_active_texture_mock_p);
    ssp_gl_set_bind_texture(ssp_gl_bind_texture_mock_p);
    ssp_gl_set_tex_image2D(ssp_gl_tex_image2D_mock_p);
    ssp_gl_set_generate_mipmap(ssp_gl_generate_mipmap_mock_p);

    ASSERT_EQ(ssp_render_bind_to_texture(images_path, &w, &h), 0);
    ASSERT_EQ(w, 960);
    ASSERT_EQ(h, 1280);
}

TEST_F(TestRender, Init_Errors)
{
    int rh;
    ASSERT_EQ(ssp_render_init(NULL), 1);

    ssp_glad_set_load_gl_loader((ssp_glad_load_gl_loader_t)ssp_glad_load_gl_loader_mock_f);
    ASSERT_EQ(ssp_render_init(ssp_resize_handler_mock), 2);
    ssp_glad_set_load_gl_loader((ssp_glad_load_gl_loader_t)ssp_glad_load_gl_loader_mock_p);

}
