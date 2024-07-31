#include <gtest/gtest.h>
#include <GLFW/glfw3.h>

extern "C" {
    #include "ssp_render.h"
}

class TestRenderGl33Texture : public ::testing::Test {
public:
    void SetUp()
    {
    }
    void TearDown()
    {
    }
};

TEST_F(TestRenderGl33Texture, GladInit_GladInitError)
{
    ssp_render_init_glad();
}
