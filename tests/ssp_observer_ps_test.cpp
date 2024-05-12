#include "gtest/gtest.h"

#include "ssp_memory.h"
#include "ssp_observer.h"
#include "ssp_observer_ps.h"

static size_t storage_count = 0;

class TestObserverPSFixture : public ::testing::Test
{
public:
    const std::string images_path = "../../tests/images/";

    observer settings;

    static const size_t storage_size = 32;
    static const size_t storage_name_len = 128;
    char* storage[storage_size];

    static void* storage_insert(void* vstorage, const char* item_name)
    {
        char* s = ((char**)vstorage)[storage_count];
        snprintf(s, storage_name_len, "%s", item_name);
        storage_count++;

        return NULL;
    }

    static void  storage_remove(void** vstorage, const char* item_name)
    {
        return;
    }

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
        for (size_t i = 0; i < storage_size; i++) {
            storage[i] = new char [storage_name_len];
            memset(storage[i], 0, storage_name_len);
        }

        settings.dirs_count = SSP_OBS_DIRS_MAX_COUNT;
        settings.storage = storage;
        settings.storage_insert = storage_insert;
        settings.storage_remove = storage_remove;
        settings.filter = filter;
        for (size_t i = 0; i < SSP_OBS_DIRS_MAX_COUNT; i++) {
            settings.dirs[i] = new char [SSP_OBS_DIR_NAME_LEN];
            snprintf(settings.dirs[i], SSP_OBS_DIR_NAME_LEN, "./directory_%li/", i);
        }
    }
    void TearDown()
    {
        storage_count = 0;

        for (size_t i = 0; i < SSP_OBS_DIRS_MAX_COUNT; i++) {
            rmdir(settings.dirs[i]);
        }

        for (size_t i = 0; i < SSP_OBS_DIRS_MAX_COUNT; i++) {
            delete(settings.dirs[i]);
        }

        for (size_t i = 0; i < storage_size; i++) {
            delete(storage[i]);
        }
    }
};

TEST_F(TestObserverPSFixture, ObserverPSInit_Success)
{
    EXPECT_EQ(ssp_obsps_init(settings), 0);
    EXPECT_EQ(ssp_ptr_storage_size(), settings.dirs_count + 1);

    ssp_obsps_destruct();
    EXPECT_EQ(ssp_ptr_storage_size(), 0);
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

    settings.dirs_count = 1;
    settings.filter = txt_filter;
    EXPECT_EQ(ssp_obsps_init(settings), 0);

    for (size_t i = 0; i < tc_count; i++) {
        FILE* f = fopen(test_cases[i], "a");
        fclose(f);

        EXPECT_EQ(ssp_obsps_process(), 0);
        for (size_t j = 0; j < max_items_in_storage; j++) {
            char* exptected_file = (char*)expected[i][j];
            char* storage_file = storage[j];
            EXPECT_STREQ(exptected_file, storage_file);
        }
    }

    ssp_obsps_destruct();
    for (size_t i = 0; i < tc_count; i++) {
        remove(test_cases[i]);
    }
}