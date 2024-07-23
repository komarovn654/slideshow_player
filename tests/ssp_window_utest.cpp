#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <GLFW/glfw3.h>

extern "C" {
    #include "ssp_graphics_mock.h"
    #include "ssp_window.h"
    #include "ssp_helper.h"
    #include "ssp_test_storage.h"
}

class TestWindowTexture : public ::testing::Test {
public:
    ssp_image_storage* image_storage;

    void SetUp()
    {
        image_storage = ssp_test_storage_init_is();
    }
    void TearDown()
    {
        ssp_ws_default_fptr();
    }
};

TEST_F(TestWindowTexture, WindowInit_InvalidDimensions)
{
    EXPECT_EQ(ssp_window_init(0, MAX_SSP_WINDOW_HEIGHT - 1, 1.0, nullptr), 1);
    EXPECT_EQ(ssp_window_init(MAX_SSP_WINDOW_WIDTH - 1, 0, 1.0, nullptr), 1);
    EXPECT_EQ(ssp_window_init(MAX_SSP_WINDOW_WIDTH + 1, MAX_SSP_WINDOW_HEIGHT - 1, 1.0, nullptr), 1);
    EXPECT_EQ(ssp_window_init(MAX_SSP_WINDOW_WIDTH - 1, MAX_SSP_WINDOW_HEIGHT + 1, 1.0, nullptr), 1);
}

TEST_F(TestWindowTexture, WindowInit_InvalidImageStorage)
{
    EXPECT_EQ(ssp_window_init(1, 1, 1.0, nullptr), 2);
}

TEST_F(TestWindowTexture, WindowInit_GlfwInitError)
{
    ssp_ws_glfw_init(ssp_glfw_init_mock_error);

    EXPECT_EQ(ssp_window_init(1, 1, 1.0, this->image_storage), 3);
}

TEST_F(TestWindowTexture, WindowInit_WindowCreateError)
{
    ssp_ws_glfw_init(ssp_glfw_init_mock_pass);
    ssp_ws_glfw_time(ssp_glfw_set_time_mock);
    ssp_ws_render_set_gl_ctx(ssp_render_set_gl_ctx_mock);
    ssp_ws_glfw_create_window(ssp_glfw_create_window_error);

    EXPECT_EQ(ssp_window_init(1, 1, 1.0, this->image_storage), 4);
}