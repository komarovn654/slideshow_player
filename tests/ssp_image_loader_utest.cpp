
#include "gtest/gtest.h"
#include "ssp_image_loader.h"

#include "../../tests/images/tiny_hex.c"

typedef struct image_info_t {
    const char* path;
    const char* data;
    int size, width, height;
} image_info;

class TestImageLoaderFixture : public ::testing::Test
{
public:
    static image_info jpg_image;

protected:
    static void SetUpTestSuite() {
        jpg_image.path = "../../tests/images/tiny.jpg";
        jpg_image.data = tiny_jpg;
        jpg_image.size = 1125;
        jpg_image.width = 25;
        jpg_image.height = 15;
    }

    static void TearDownTestSuite() {
        //code here
    }

    void SetUp()
    {
    }
    void TearDown()
    {
    }
};

TEST(TestImageLoader, IL_ReadJPG) 
{
    ssp_image* im = ssp_il_read_image(TestImageLoaderFixture::jpg_image.path);
    printf("size: %i\n", TestImageLoaderFixture::jpg_image.size);
    printf("width: %i\n", TestImageLoaderFixture::jpg_image.width);
    printf("height: %i\n"), TestImageLoaderFixture::jpg_image.height;
}
