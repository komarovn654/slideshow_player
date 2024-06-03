#include <cstdlib>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>

#include "gtest/gtest.h"
#include "ssp_helper.h"

extern "C" {
    ssp_static int ssp_dir_absolute_path(const char* dir_path, char* abs_path);
}

class TestHelperFixture : public ::testing::Test
{
public:
    static const size_t max_count = 10;
    const std::string dir_path = "../../tests/images/";
    const std::string empty_dir_path = "./empty_dir";
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
        rmdir(empty_dir_path.data());
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

void* test_store_files(void *storage, const char *file_name)
{
    static size_t row_num = 0;
    snprintf((char *)storage + (row_num++) * SSP_FILE_NAME_MAX_LEN, SSP_FILE_NAME_MAX_LEN, "%s", file_name);

    return NULL;
}

TEST_F(TestHelperFixture, DirTraversal) 
{
    char storage[max_count][SSP_FILE_NAME_MAX_LEN] = { 0 };
    
    ASSERT_EQ(ssp_dir_traversal(dir_path.data(), test_store_files, storage, ssp_is_file_image), 0);

    size_t i, j = 0;
    for(i = 0; i < jpg_files_count; i++) {
        for(j = 0; j < jpg_files_count; j++) {
            std::string storage_fullname(storage[i]);
            std::string storage_filename = storage_fullname.substr(storage_fullname.find_last_of("/\\") + 1);
            std::string expected_filename = jpg_files[j].substr(jpg_files[j].find_last_of("/\\") + 1);

            if (!storage_filename.compare(expected_filename)) {
                // printf("%s %s\n", (char *)storage_filename.data(), (char *)expected_filename.data());
                ASSERT_STREQ(storage_filename.data(), expected_filename.data());
                j = jpg_files_count + 1;
                break;
            }
        }
        if (j != jpg_files_count + 1) {
            FAIL() << storage[i];
        }
    }
}

TEST_F(TestHelperFixture, DirTraversal_Errors) 
{   
    char storage[max_count][SSP_FILE_NAME_MAX_LEN] = { 0 };

    ASSERT_EQ(ssp_dir_traversal(NULL, test_store_files, storage, ssp_is_file_image), 1);
    ASSERT_EQ(ssp_dir_traversal(dir_path.data(), NULL, storage, ssp_is_file_image), 1);
    ASSERT_EQ(ssp_dir_traversal("dir", test_store_files, storage, ssp_is_file_image), 1);
}

TEST_F(TestHelperFixture, DirTraversal_Empty) 
{   
    char storage[max_count][SSP_FILE_NAME_MAX_LEN] = { 0 };
    
    ASSERT_NE(ssp_mkdir(empty_dir_path.data()), -1);

    ASSERT_EQ(ssp_dir_traversal(empty_dir_path.data(), test_store_files, storage, ssp_is_file_image), 0);
    
    size_t i = 0;
    for(i = 0; i < jpg_files_count; i++) {
        ASSERT_STREQ(storage[i], "");
    }
}

TEST_F(TestHelperFixture, AbsPath)
{
    char expected[PATH_MAX];
    ssp_mkdir(empty_dir_path.data());

    snprintf(expected, 200, "%s %s", SSP_EXC_PATH, (char*)empty_dir_path.data()[1]);
    printf("%s\n", expected);

    ssp_dir_absolute_path((const char*)empty_dir_path.data(), expected);
    printf("%s\n", SSP_EXC_PATH);
    printf("%s\n", expected);
}
