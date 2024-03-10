#include "gtest/gtest.h"

class TestListFixture : public ::testing::Test
{
protected:
    void SetUp()
    {
    }
    void TearDown()
    {
    }
};

TEST_F(TestListFixture, ListInitialization) 
{
    EXPECT_TRUE(true);
}
