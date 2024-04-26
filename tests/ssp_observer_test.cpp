#include <cstdlib>
#include <string>
#include <dirent.h>

#include "gtest/gtest.h"
#include "ssp_observer.h"

class TestObserverFixture : public ::testing::Test
{
public:
    static const size_t max_count = 10;
    const std::string dir_path = "../../tests/images/";
    const std::string tmp_dir_path = "../../tests/tmp_dir/";
    size_t jpg_files_count = 0;
    std::string jpg_files[max_count];


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
        };
    }
    void TearDown()
    {
        rmdir(tmp_dir_path.data());
    }
};

TEST_F(TestObserverFixture, ObserverImagesList_Errors) 
{
    char images[max_count][SSP_FILE_NAME_MAX_LEN];

    ASSERT_EQ(ssp_obs_images_list((char **)images), 1);
}

TEST_F(TestObserverFixture, ObserverImagesList_Empty) 
{
    char images[max_count][SSP_FILE_NAME_MAX_LEN] = { 0 };
    ssp_dir_create(tmp_dir_path.data());

    ssp_obs_init(tmp_dir_path.data());

    ASSERT_EQ(ssp_obs_images_list((char **)images), 0);
    for (size_t i = 0; i < max_count; i++) {
        ASSERT_STREQ(images[i], "");
    }
}

TEST_F(TestObserverFixture, ObserverImagesList) 
{
    char images[max_count][SSP_FILE_NAME_MAX_LEN] = { 0 };

    ssp_obs_init(dir_path.data());

    ASSERT_EQ(ssp_obs_images_list((char **)images), 0);
    
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