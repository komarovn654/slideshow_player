#include <cstdlib>
#include <string>
#include <dirent.h>

#include "gtest/gtest.h"
#include "ssp_helper.h"

class TestHelperFixture : public ::testing::Test
{
public:
    static const size_t max_count = 10;
    const std::string dir_path = "../../tests/images/";
    size_t jpg_files_count = 0;
    size_t txt_files_count = 0;
    std::string jpg_files[max_count];
    std::string txt_files[max_count];
protected:
    void SetUp()
    {
        DIR *jdir = opendir(dir_path.data());
        if (jdir == NULL) {
            return;
        }
        
        struct dirent *jd;
        while ((jd = readdir(jdir)) != NULL) {
            if (((std::string)jd->d_name).find(".jpg") != std::string::npos) {
                jpg_files[jpg_files_count++] = dir_path + (std::string)jd->d_name;
            }
            if (((std::string)jd->d_name).find(".txt") != std::string::npos) {
                txt_files[txt_files_count++] = dir_path + (std::string)jd->d_name;
            }
        };
    }
    void TearDown()
    {
    }
};

TEST(TestHelper, IsImage_Errors) 
{
    ASSERT_FALSE(ssp_is_file_image(NULL));
    ASSERT_FALSE(ssp_is_file_image("123"));
}

TEST_F(TestHelperFixture, IsImage_JPG) 
{
    for(size_t j = 0; j < jpg_files_count; j++) {
        ASSERT_TRUE(ssp_is_file_image((const char *)jpg_files[j].data()));
    }
}

TEST_F(TestHelperFixture, IsImage_TXT) 
{
    for(size_t j = 0; j < txt_files_count; j++) {
        ASSERT_FALSE(ssp_is_file_image((const char *)txt_files[j].data()));
    }
}