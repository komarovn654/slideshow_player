#include "gtest/gtest.h"
#include "ssp_image_loader.h"
#include "ssp_memory.h"

#include "../../tests/images/tiny_hex.c"

typedef struct image_info_t {
    const char* path;
    const unsigned char* data;
    int buf_size, width, height;
} image_info;

class TestImageLoaderFixture : public ::testing::Test
{
public:
    static image_info jpg_image;

protected:
    static void SetUpTestSuite() 
    {
        jpg_image.path = "../../tests/images/tiny.jpg";
        jpg_image.data = tiny_jpg;
        jpg_image.buf_size = 1125;
        jpg_image.width = 25;
        jpg_image.height = 15;
    }
};

image_info TestImageLoaderFixture::jpg_image;

TEST_F(TestImageLoaderFixture, ILRead_NULL)
{
    ASSERT_TRUE(ssp_il_read_image(NULL) == NULL);
    ASSERT_TRUE(ssp_il_read_image("non-existent") == NULL);
    
    ASSERT_EQ(ssp_ptr_storage_size(), 0);
}

TEST_F(TestImageLoaderFixture, ILDelete)
{
    ssp_image* im = ssp_il_read_image(TestImageLoaderFixture::jpg_image.path);
    ASSERT_EQ(ssp_ptr_storage_size(), 1);

    ssp_il_delete_image(im);
    ASSERT_EQ(ssp_ptr_storage_size(), 0);
}

TEST_F(TestImageLoaderFixture, ILRead_JPG)
{
    ssp_image* im = ssp_il_read_image(TestImageLoaderFixture::jpg_image.path);
    ASSERT_EQ(TestImageLoaderFixture::jpg_image.buf_size, im->buf_size);
    ASSERT_EQ(TestImageLoaderFixture::jpg_image.width, im->width);
    ASSERT_EQ(TestImageLoaderFixture::jpg_image.height, im->height);
    // ASSERT_TRUE(std::equal(im->data, im->data + im->buf_size, TestImageLoaderFixture::jpg_image.data));
    // for (size_t i = 0; i < im->buf_size;i++) {
    //     if (im->data[i] != TestImageLoaderFixture::jpg_image.data[i]) {
    //         printf("0x%X 0x%x ", im->data[i], TestImageLoaderFixture::jpg_image.data[i]);
    //         printf("%lu\n", i);
    //     }
    // }
}
