#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <GLFW/glfw3.h>

extern "C" {
    #include "ssp_window.h"

    // MOCK_FUNCTION(int, ssp_glfw_init, (ssp_display_platform), ());
    // MOCK_FUNCTION(int)
    // MOCK_FUNCTION(void, ssp_render_set_gl_ctx, (), ());
    // MOCK_FUNCTION(GLFWwindow*, glfwCreateWindow, (int, int, const char*, GLFWmonitor*, GLFWwindow*), ());
    // MOCK_FUNCTION(void, glfwMakeContextCurrent, (GLFWwindow*), ());
    // MOCK_FUNCTION(int, ssp_render_init, (void (*)(int, int)), ());
    // MOCK_FUNCTION(const GLubyte*, glGetString, (GLenum), ());
    // MOCK_FUNCTION(void, glfwSetTime, (double), ());
    // MOCK_FUNCTION(void, syslog, (int, const char*, ...), ());
}

class MockFunctions {
public:
    MOCK_METHOD(int, ssp_glfw_init, (ssp_display_platform), ());
    MOCK_METHOD(void, syslog, (int, const char*, ...), ());
};

class TestWindowTexture : public ::testing::Test {
protected:
    void SetUp() override {
    }
};

TEST_F(TestWindowTexture, InvalidDimensions) {
    // EXPECT_CALL(syslog, _).Times(1);
    
    EXPECT_EQ(ssp_window_init(0, 1080, 1.0, nullptr), 1);
    EXPECT_EQ(ssp_window_init(1920, 0, 1.0, nullptr), 1);
    EXPECT_EQ(ssp_window_init(1921, 1080, 1.0, nullptr), 1);
    EXPECT_EQ(ssp_window_init(1920, 1081, 1.0, nullptr), 1);
}