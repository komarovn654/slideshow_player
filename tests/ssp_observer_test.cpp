#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "gtest/gtest.h"
#include "ssp_list.h"
#include "ssp_observer.h"
#include "ssp_memory.h"

extern "C" 
{
    ssp_static int ssp_obs_assert(observer settings);
}

static size_t storage_count = 0;

class TestObserverFixture : public ::testing::Test
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

    static void storage_remove(void** vstorage, const char* item_name)
    {
        return;
    }

    static bool filter(const char *file_name)
    {
        const char filter_name[15] = "you won't pass";

        if (strcmp(filter_name, file_name) == 0) {
            return false;
        }
        return true;
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

TEST_F(TestObserverFixture, ObserverAssert_Success)
{
    EXPECT_EQ(ssp_obs_assert(settings), 0);
}

TEST_F(TestObserverFixture, ObserverAssert_Error)
{
    char* d = settings.dirs[0];
    settings.dirs[0] = NULL;
    EXPECT_EQ(ssp_obs_assert(settings), 1);
    settings.dirs[0] = d;
    
    settings.dirs_count = 0;
    EXPECT_EQ(ssp_obs_assert(settings), 1);
    settings.dirs_count = SSP_OBS_DIRS_MAX_COUNT;

    settings.dirs_count = 11;
    EXPECT_EQ(ssp_obs_assert(settings), 1);
    settings.dirs_count = SSP_OBS_DIRS_MAX_COUNT;    

    settings.storage = NULL;
    EXPECT_EQ(ssp_obs_assert(settings), 1);
    settings.storage = storage;

    settings.storage_insert = NULL;
    EXPECT_EQ(ssp_obs_assert(settings), 1);
    settings.storage_insert = storage_insert;

    settings.storage_remove = NULL;
    EXPECT_EQ(ssp_obs_assert(settings), 1);
    settings.storage_remove = storage_remove;

    settings.filter = NULL;
    EXPECT_EQ(ssp_obs_assert(settings), 1);
    settings.filter = filter;    
}

TEST_F(TestObserverFixture, ObserverInit_Error)
{
    observer err_settings = { };
    EXPECT_TRUE(ssp_obs_init(err_settings) == NULL);
    EXPECT_EQ(ssp_ptr_storage_size(), 0);
}

TEST_F(TestObserverFixture, ObserverInit_Success)
{
    observer* obs = ssp_obs_init(settings);
    EXPECT_TRUE(obs != NULL);

    EXPECT_EQ(obs->dirs_count, settings.dirs_count);
    EXPECT_EQ(obs->storage, settings.storage);
    EXPECT_EQ(obs->storage_insert, settings.storage_insert);
    EXPECT_EQ(obs->storage_remove, settings.storage_remove);
    EXPECT_EQ(obs->filter, settings.filter);

    for (size_t i = 0; i < settings.dirs_count; i++) {
        EXPECT_STREQ(obs->dirs[i], settings.dirs[i]);
    }

    EXPECT_EQ(ssp_ptr_storage_size(), settings.dirs_count + 1);
    ssp_obs_destruct(obs);
}

TEST_F(TestObserverFixture, ObserverDestruct)
{
    observer* obs = ssp_obs_init(settings);
    EXPECT_TRUE(obs != NULL);

    EXPECT_EQ(ssp_ptr_storage_size(), settings.dirs_count + 1);
    ssp_obs_destruct(obs);
    EXPECT_EQ(ssp_ptr_storage_size(), 0);
}

TEST_F(TestObserverFixture, ObserverStorageInsert_NULL)
{
    EXPECT_TRUE(ssp_obs_storage_insert(NULL, "item") == NULL);
}

TEST_F(TestObserverFixture, ObserverStorageInsert_SSPListInsert)
{
    const size_t tc_count = 4;
    const size_t item_count = 4;
    const size_t item_len = 10;

    char list_storage[item_count][item_len] = { "" };
    const char expected[tc_count][item_count][item_len] = {
        {"",       "",       "",       ""},
        {"", "item_0",       "",       ""},
        {"", "item_0", "item_1",       ""},
        {"", "item_0", "item_1", "item_2"},
    };
    const char items_to_insert[tc_count][item_len] = {"", "item_0", "item_1", "item_2"};

    settings.storage = ssp_list_init();
    settings.storage_insert = ssp_list_insertv;
    observer* obs = ssp_obs_init(settings);
    EXPECT_TRUE(obs != NULL);

    for (size_t i = 0; i < tc_count; i++) {
        EXPECT_TRUE(ssp_obs_storage_insert(obs, items_to_insert[i]) != NULL);

        ssp_list_traversal((ssp_list)settings.storage, (char **)list_storage, item_len);
        EXPECT_STREQ(list_storage[0], expected[i][0]);
        EXPECT_STREQ(list_storage[1], expected[i][1]);
        EXPECT_STREQ(list_storage[2], expected[i][2]);
        EXPECT_STREQ(list_storage[3], expected[i][3]);
    }

    ssp_list_destruct((ssp_list)settings.storage);
    ssp_obs_destruct(obs);
}

TEST_F(TestObserverFixture, ObserverStorageRemove_SSPListRemove)
{
    const size_t tc_count = 6;
    const size_t item_count = 4;
    const size_t item_len = 10;

    char list_storage[item_count][item_len] = { "" };
    const char expected[tc_count][item_count][item_len] = {
        {"item_0", "item_1", "item_2", "item_3"},
        {"item_0", "item_1", "item_2", "item_3"},
        {"item_0", "item_1", "item_3",       ""},
        {"item_0", "item_3",       "",       ""},
        {"item_0",       "",       "",       ""},
        {      "",       "",       "",       ""},
    };
    const char items_to_remove[tc_count][item_len] = {"smth", "", "item_2", "item_1", "item_3", "item_0"};

    settings.storage = ssp_list_init();
    settings.storage_insert = ssp_list_insertv;
    settings.storage_remove = ssp_list_removev_node;
    observer* obs = ssp_obs_init(settings);
    EXPECT_TRUE(obs != NULL);

    ssp_obs_storage_insert(obs, "item_0");
    ssp_obs_storage_insert(obs, "item_1");
    ssp_obs_storage_insert(obs, "item_2");
    ssp_obs_storage_insert(obs, "item_3");

    for (size_t i = 0; i < tc_count; i++) {
        ssp_obs_storage_remove(obs, items_to_remove[i]);

        memset(list_storage, 0, item_count * item_len);
        ssp_list_traversal((ssp_list)settings.storage, (char **)list_storage, item_len);
        EXPECT_STREQ(list_storage[0], expected[i][0]);
        EXPECT_STREQ(list_storage[1], expected[i][1]);
        EXPECT_STREQ(list_storage[2], expected[i][2]);
        EXPECT_STREQ(list_storage[3], expected[i][3]);
    }

    ssp_list_destruct((ssp_list)settings.storage);
    ssp_obs_destruct(obs);
}

TEST_F(TestObserverFixture, ObserverFilterFunction)
{
    const size_t tc_count = 3;
    const char test_cases[tc_count][15] = {"you won't pass", "pass", "one more pass"};
    const bool expected[tc_count] = {false, true, true};

    observer* obs = ssp_obs_init(settings);

    for (size_t i = 0; i < tc_count; i++) {
        EXPECT_EQ(ssp_obs_filter(obs, test_cases[i]), expected[i]);
    }

    ssp_obs_destruct(obs);
}

TEST_F(TestObserverFixture, ObserverCreateDirectories)
{
    snprintf(settings.dirs[0], SSP_OBS_DIR_NAME_LEN, "%s", images_path.data());
    settings.dirs_count = 3;
    observer* obs = ssp_obs_init(settings);

    EXPECT_EQ(ssp_obs_dirs_create(obs), 0);

    for (size_t i = 0; i < settings.dirs_count; i++) {
        struct stat path_stat;
        stat(settings.dirs[0], &path_stat);
        EXPECT_EQ(S_ISDIR(path_stat.st_mode), 1);
    }

    ssp_obs_destruct(obs);
}

TEST_F(TestObserverFixture, ObserverTraversalDirectories)
{
    /* Filter will be set only for files with the substring ".txt",
        so {"test1.txt", "test2.txt"} from <test/images/> 
        and test3.txt from <tests/directory_1> are expected. */
    const size_t expected_count = 3;
    char expected[expected_count][SSP_OBS_DIR_NAME_LEN] = {
        "../../tests/images/test1.txt",
        "../../tests/images/test2.txt",
    };
    char third_file[SSP_OBS_DIR_NAME_LEN];
    snprintf(third_file, SSP_OBS_DIR_NAME_LEN, "%stest3.txt", settings.dirs[1]);
    snprintf(expected[2], SSP_OBS_DIR_NAME_LEN, "%s", third_file);

    snprintf(settings.dirs[0], SSP_OBS_DIR_NAME_LEN, "%s", images_path.data());
    settings.dirs_count = 3;
    settings.filter = txt_filter;
    observer* obs = ssp_obs_init(settings);
    ssp_obs_dirs_create(obs);

    FILE* f = fopen(third_file ,"a");
    fclose(f);

    EXPECT_EQ(ssp_obs_dirs_traversal(obs), 0);
    // sort storage by names
    std::sort((const char**)storage, (const char**)storage + storage_count, [](const char* a, const char* b) {
        return std::strcmp(a, b) < 0;
    });

    for (size_t i = 0; i < expected_count; i++) {
        EXPECT_STREQ(expected[i], storage[i]);
    }

    remove(third_file);
    ssp_obs_destruct(obs);
}
