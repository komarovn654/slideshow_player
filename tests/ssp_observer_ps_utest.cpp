#include <algorithm>
#include <cstring>
#include <chrono>
#include <thread>
#include "gtest/gtest.h"

#include "ssp_memory.h"
#include "ssp_test_storage.h"
#include "ssp_observer.h"
#include "ssp_observer_ps.h"

class TestObserverPSFixture : public ::testing::Test
{
public:
    ssp_observer settings;
    ssp_image_storage* is;
    const size_t is_size = 22;

    static std::string cut_fullname(const char* item_name)
    {
        std::string name_str(item_name);
        std::string base_name_str = name_str.substr(name_str.find_last_of("/\\") + 1);

        return base_name_str;
    }

    // static void* storage_insert(void* vstorage, const char* item_name)
    // {
    //     char* s = ((char**)vstorage)[storage_count];
    //     snprintf(s, storage_name_len, "%s", item_name);
    //     storage_count++;

    //     return NULL;
    // }

    // static void  storage_remove(void** vstorage, const char* item_name)
    // {
    //     for (size_t i = 0; i < storage_count; i++) {
    //         char* s = ((char**)vstorage)[i];
    //         if (std::strcmp(cut_fullname(s).data(), cut_fullname(item_name).data()) == 0) {
    //             memset(s, 0, storage_name_len);
    //             return;
    //         }
    //     }

    //     return;
    // }

    static bool filter(const char *file_name)
    {
        return false;
    }

    static bool txt_filter(const char *file_name)
    {
        std::string txt_file = ".txt";
        std::string file(file_name);

        if (file.find(txt_file) != std::string::npos) {
            return true;
        }

        return false;
    }

protected:
    void SetUp()
    {
        is = ssp_test_storage_init();

        settings.dirs_count = SSP_OBS_DIRS_MAX_COUNT;
        settings.filter = filter;
        for (size_t i = 0; i < SSP_OBS_DIRS_MAX_COUNT; i++) {
            settings.dirs[i] = new char [PATH_MAX];
            snprintf(settings.dirs[i], PATH_MAX, "./directory_%li/", i);
        }
    }
    void TearDown()
    {
        for (size_t i = 0; i < SSP_OBS_DIRS_MAX_COUNT; i++) {
            rmdir(settings.dirs[i]);
        }

        for (size_t i = 0; i < SSP_OBS_DIRS_MAX_COUNT; i++) {
            delete(settings.dirs[i]);
        }

        ssp_test_storage_destruct(is);
    }
};

TEST_F(TestObserverPSFixture, ObserverPSInit_Success)
{
    settings.istorage = is;
    EXPECT_EQ(ssp_obsps_init(settings), 0);
    EXPECT_EQ(ssp_ptr_storage_size(), settings.dirs_count + 1 + is_size);

    ssp_obsps_destruct();
    EXPECT_EQ(ssp_ptr_storage_size(), is_size);
}

TEST_F(TestObserverPSFixture, ObserverPSProcess_Create)
{
    const size_t tc_count = 5;
    const size_t max_items_in_storage = 3;
    const char test_cases[tc_count][SSP_FILE_NAME_MAX_LEN] = {
        "./directory_0/text_file.txt",
        "./directory_0/image_file1.jpg",
        "./directory_0/image_file2.png",
        "./directory_0/image_file1.jpeg",
        "./directory_0/one_more_text.txt",
    };
    const char expected[tc_count][max_items_in_storage][SSP_FILE_NAME_MAX_LEN] = {
        {"./directory_0/text_file.txt", "", ""},
        {"./directory_0/text_file.txt", "", ""},
        {"./directory_0/text_file.txt", "", ""},
        {"./directory_0/text_file.txt", "", ""},
        {"./directory_0/text_file.txt", "./directory_0/one_more_text.txt", ""},
    };

    settings.istorage = is;
    settings.dirs_count = 1;
    settings.filter = txt_filter;
    EXPECT_EQ(ssp_obsps_init(settings), 0);

    for (size_t i = 0; i < tc_count; i++) {
        FILE* f = fopen(test_cases[i], "a");
        fclose(f);

        for (size_t i = 0; i < 10; i++) {
            EXPECT_EQ(ssp_obsps_process(), 0);
        }

        for (size_t j = 0; j < max_items_in_storage; j++) {
            EXPECT_STREQ(cut_fullname((char*)expected[i][j]).data(), 
                cut_fullname(((char**)settings.istorage->storage_head)[j]).data());
        }
    }

    ssp_obsps_destruct();
    for (size_t i = 0; i < tc_count; i++) {
        remove(test_cases[i]);
    }
}

TEST_F(TestObserverPSFixture, ObserverPSProcess_Remove)
{
    const size_t tc_count = 6;
    const char test_cases[tc_count][SSP_FILE_NAME_MAX_LEN] = {
        "./directory_0/text_file_1.txt",
        "./directory_0/image_file1.jpg",
        "./directory_0/image_file2.png",
        "./directory_0/image_file1.jpeg",
        "./directory_0/text_file_2.txt",
        "./directory_0/text_file_3.txt"
    };

    settings.istorage = is;
    settings.dirs_count = 1;
    settings.filter = txt_filter;

    // Create tests files
    ssp_dir_create("./directory_0/");
    for (size_t i = 0; i < tc_count; i++) {
        FILE* f = fopen(test_cases[i], "a");
        fclose(f);
    }
    // Wait for the files to be created
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    // Initialize obs
    EXPECT_EQ(ssp_obsps_init(settings), 0);

    for (size_t i = 0; i < tc_count; i++) {
        remove(test_cases[i]);

        for (size_t i = 0; i < 10; i++) {
            EXPECT_EQ(ssp_obsps_process(), 0);
        }

        for (size_t j = 0; j < SSP_TS_MAX_ITEM_COUNT; j++) {
            std::string storage_item = cut_fullname(((char**)settings.istorage->storage_ptr)[j]);
            std::string removed_item = cut_fullname(test_cases[i]);
            EXPECT_STRNE(storage_item.data(), removed_item.data());
        }
    }

    ssp_obsps_destruct();
}

TEST_F(TestObserverPSFixture, ObserverPSProcess_MultiDirs)
{
    const size_t tc_count = 10;
    const size_t max_items_in_storage = 10;
    const char test_cases[tc_count][SSP_FILE_NAME_MAX_LEN] = {
        "f0.txt",
        "f1.txt",
        "f2.txt",
        "f3.txt",
        "f4.txt",
        "f5.txt",
        "f6.txt",
        "f7.txt",
        "f8.txt",
        "f9.txt",
    };
    const char expected[tc_count][max_items_in_storage][SSP_FILE_NAME_MAX_LEN] = {
        {"f0.txt", "", "", "", "", "", "", "", "", ""},
        {"f0.txt", "f1.txt", "", "", "", "", "", "", "", ""},
        {"f0.txt", "f1.txt", "f2.txt", "", "", "", "", "", "", ""},
        {"f0.txt", "f1.txt", "f2.txt", "f3.txt", "", "", "", "", "", ""},
        {"f0.txt", "f1.txt", "f2.txt", "f3.txt", "f4.txt", "", "", "", "", ""},
        {"f0.txt", "f1.txt", "f2.txt", "f3.txt", "f4.txt", "f5.txt", "", "", "", ""},
        {"f0.txt", "f1.txt", "f2.txt", "f3.txt", "f4.txt", "f5.txt", "f6.txt", "", "", ""},
        {"f0.txt", "f1.txt", "f2.txt", "f3.txt", "f4.txt", "f5.txt", "f6.txt", "f7.txt", "", ""},
        {"f0.txt", "f1.txt", "f2.txt", "f3.txt", "f4.txt", "f5.txt", "f6.txt", "f7.txt", "f8.txt", ""},
        {"f0.txt", "f1.txt", "f2.txt", "f3.txt", "f4.txt", "f5.txt", "f6.txt", "f7.txt", "f8.txt", "f9.txt"},
    };

    settings.istorage = is;
    settings.dirs_count = 10;
    settings.filter = txt_filter;
    EXPECT_EQ(ssp_obsps_init(settings), 0);

    for (size_t i = 0; i < tc_count; i++) {
        char file_name[SSP_FILE_NAME_MAX_LEN];
        snprintf(file_name, SSP_FILE_NAME_MAX_LEN, "%s%s", settings.dirs[i], test_cases[i]);
        FILE* f = fopen(file_name, "a");
        fclose(f);

        for (size_t i = 0; i < 10; i++) {
            EXPECT_EQ(ssp_obsps_process(), 0);
        }

        for (size_t j = 0; j < max_items_in_storage; j++) {
            EXPECT_STREQ(cut_fullname((char*)expected[i][j]).data(), 
                cut_fullname(((char**)settings.istorage->storage_ptr)[j]).data());
        }
    }

    for (size_t i = tc_count - 1; i > 0; i--) {
        char file_name[SSP_FILE_NAME_MAX_LEN];
        snprintf(file_name, SSP_FILE_NAME_MAX_LEN, "%s%s", settings.dirs[i], test_cases[i]);
        remove(file_name);

        for (size_t i = 0; i < 10; i++) {
            EXPECT_EQ(ssp_obsps_process(), 0);
        }

        for (size_t j = 0; j < max_items_in_storage; j++) {
            EXPECT_STREQ(cut_fullname((char*)expected[i - 1][j]).data(),
                cut_fullname(((char**)settings.istorage->storage_ptr)[j]).data());
        }
    }

    ssp_obsps_destruct();
    char file_name[SSP_FILE_NAME_MAX_LEN];
    snprintf(file_name, SSP_FILE_NAME_MAX_LEN, "%s%s", settings.dirs[0], test_cases[0]);    
    remove(file_name);
}
