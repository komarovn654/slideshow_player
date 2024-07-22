#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <GLFW/glfw3.h>

extern "C" {
    #include "ssp_graphics_mock.h"
    #include "ssp_window.h"
    #include "ssp_helper.h"
}

class TestWindowTexture : public ::testing::Test {
protected:
    void SetUp()
    {
    }
    void TearDown()
    {
        ssp_window_set_default_fptr();
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
    EXPECT_EQ(ssp_window_init(1, 1, 1.0, nullptr), 1);
}

TEST_F(TestWindowTexture, WindowInit_GlfwInitError)
{
    ssp_window_set_glfw_init(ssp_glfw_init_mock_error);

    EXPECT_EQ(ssp_window_init(1, 1, 1.0, nullptr), 1);
}