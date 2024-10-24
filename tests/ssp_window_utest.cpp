#include <gtest/gtest.h>

extern "C" {
    #include "ssp_glfw.h"
}

class TestWindowTexture : public ::testing::Test {
public:
    void SetUp()
    {
    }
    void TearDown()
    {
    }
};

TEST_F(TestWindowTexture, WindowInit_InvalidDimensions)
{

}
