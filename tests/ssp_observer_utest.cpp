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
#include "ssp_test_storage.h"
#include "ssp_memory.h"

extern "C" 
{
    ssp_static int ssp_obs_assert(ssp_observer settings);
}

class TestObserverFixture : public ::testing::Test
{
public:
    const std::string images_path = "../../tests/images/";

    ssp_observer settings;
    ssp_image_storage* is;

    static std::string cut_fullname(const char* item_name)
    {
        std::string name_str(item_name);
        std::string base_name_str = name_str.substr(name_str.find_last_of("/\\") + 1);

        return base_name_str;
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
        is = ssp_test_storage_init_is();

        settings.dirs_count = SSP_OBS_DIRS_MAX_COUNT;
        settings.filter = filter;
        for (size_t i = 0; i < SSP_OBS_DIRS_MAX_COUNT; i++) {
            settings.dirs[i] = new char [SSP_PATH_MAX_LEN];
            snprintf(settings.dirs[i], SSP_PATH_MAX_LEN, "./directory_%li/", i);
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

        ssp_test_storage_destruct_is(is);
    }
};

TEST_F(TestObserverFixture, ObserverAssert_Success)
{
    settings.istorage = is;
    EXPECT_EQ(ssp_obs_assert(settings), 0);
}

TEST_F(TestObserverFixture, ObserverAssert_Error)
{
    ssp_observer tmp_obs;
    memcpy(&tmp_obs, &settings, sizeof(ssp_observer));
    tmp_obs.istorage = is;

    tmp_obs.dirs[0] = NULL;
    EXPECT_EQ(ssp_obs_assert(tmp_obs), 1);
    tmp_obs.dirs[0] = settings.dirs[0];
    
    tmp_obs.dirs_count = 0;
    EXPECT_EQ(ssp_obs_assert(tmp_obs), 1);
    tmp_obs.dirs_count = SSP_OBS_DIRS_MAX_COUNT;

    tmp_obs.dirs_count = 11;
    EXPECT_EQ(ssp_obs_assert(tmp_obs), 1);
    tmp_obs.dirs_count = SSP_OBS_DIRS_MAX_COUNT;    

    tmp_obs.istorage = NULL;
    EXPECT_EQ(ssp_obs_assert(tmp_obs), 1);
    tmp_obs.istorage = is;

    tmp_obs.filter = NULL;
    EXPECT_EQ(ssp_obs_assert(tmp_obs), 1);
    tmp_obs.filter = filter;
}

TEST_F(TestObserverFixture, ObserverInit_Error)
{
    size_t used_ptr = ssp_ptr_storage_size();
    ssp_observer err_settings = { };
    EXPECT_TRUE(ssp_obs_init(err_settings) == NULL);
    EXPECT_EQ(ssp_ptr_storage_size(), used_ptr);
}

TEST_F(TestObserverFixture, ObserverInit_Success)
{
    size_t used_ptr = ssp_ptr_storage_size();

    settings.istorage = is;
    ssp_observer* obs = ssp_obs_init(settings);
    EXPECT_TRUE(obs != NULL);

    EXPECT_EQ(obs->dirs_count, settings.dirs_count);
    EXPECT_EQ(obs->istorage, settings.istorage);
    EXPECT_EQ(obs->filter, settings.filter);

    for (size_t i = 0; i < settings.dirs_count; i++) {
        EXPECT_STREQ(obs->dirs[i], settings.dirs[i]);
    }

    EXPECT_EQ(ssp_ptr_storage_size(), settings.dirs_count + 1 + used_ptr);
    ssp_obs_destruct(obs);
}

TEST_F(TestObserverFixture, ObserverDestruct)
{
    size_t used_ptr = ssp_ptr_storage_size();

    settings.istorage = is;
    ssp_observer* obs = ssp_obs_init(settings);
    EXPECT_TRUE(obs != NULL);

    EXPECT_EQ(ssp_ptr_storage_size(), settings.dirs_count + 1 + used_ptr);
    ssp_obs_destruct(obs);
    EXPECT_EQ(ssp_ptr_storage_size(), used_ptr);
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

    char list_storage[item_count][item_len];
    memset(list_storage, 0, item_count * item_len);
    const char expected[tc_count][item_count][item_len] = {
        {"",       "",       "",       ""},
        {"", "item_0",       "",       ""},
        {"", "item_0", "item_1",       ""},
        {"", "item_0", "item_1", "item_2"},
    };
    const char items_to_insert[tc_count][item_len] = {"", "item_0", "item_1", "item_2"};

    settings.istorage = ssp_list_init_is();
    ssp_observer* obs = ssp_obs_init(settings);
    EXPECT_TRUE(obs != NULL);

    for (size_t i = 0; i < tc_count; i++) {
        EXPECT_TRUE(obs->istorage->insert(obs->istorage->storage, items_to_insert[i]) != NULL);

        ssp_list_traversal((ssp_list)settings.istorage->storage, (char **)list_storage, item_len);
        EXPECT_STREQ(list_storage[0], expected[i][0]);
        EXPECT_STREQ(list_storage[1], expected[i][1]);
        EXPECT_STREQ(list_storage[2], expected[i][2]);
        EXPECT_STREQ(list_storage[3], expected[i][3]);
    }

    ssp_list_destruct_is(settings.istorage);
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

    settings.istorage = ssp_list_init_is();
    ssp_observer* obs = ssp_obs_init(settings);
    EXPECT_TRUE(obs != NULL);

    ssp_obs_storage_insert(obs, "item_0");
    ssp_obs_storage_insert(obs, "item_1");
    ssp_obs_storage_insert(obs, "item_2");
    ssp_obs_storage_insert(obs, "item_3");

    for (size_t i = 0; i < tc_count; i++) {
        ssp_obs_storage_remove(obs, items_to_remove[i]);

        memset(list_storage, 0, item_count * item_len);
        ssp_list_traversal((ssp_list)settings.istorage->storage, (char **)list_storage, item_len);
        EXPECT_STREQ(list_storage[0], expected[i][0]);
        EXPECT_STREQ(list_storage[1], expected[i][1]);
        EXPECT_STREQ(list_storage[2], expected[i][2]);
        EXPECT_STREQ(list_storage[3], expected[i][3]);
    }

    ssp_list_destruct_is(settings.istorage);
    ssp_obs_destruct(obs);
}

TEST_F(TestObserverFixture, ObserverFilterFunction)
{
    const size_t tc_count = 3;
    const char test_cases[tc_count][15] = {"you won't pass", "pass", "one more pass"};
    const bool expected[tc_count] = {false, true, true};

    settings.istorage = is;
    ssp_observer* obs = ssp_obs_init(settings);

    for (size_t i = 0; i < tc_count; i++) {
        EXPECT_EQ(ssp_obs_filter(obs, test_cases[i]), expected[i]);
    }

    ssp_obs_destruct(obs);
}

TEST_F(TestObserverFixture, ObserverCreateDirectories)
{
    snprintf(settings.dirs[0], SSP_PATH_MAX_LEN, "%s", images_path.data());
    settings.dirs_count = 3;

    settings.istorage = is;
    ssp_observer* obs = ssp_obs_init(settings);

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
    char expected[expected_count][SSP_FULL_NAME_MAX_LEN] = {
        "../../tests/images/test1.txt",
        "../../tests/images/test2.txt",
        "./directory_1/test3.txt"
    };

    snprintf(settings.dirs[0], SSP_FULL_NAME_MAX_LEN, "%s", images_path.data());
    settings.dirs_count = 3;
    settings.filter = txt_filter;
    settings.istorage = is;
    ssp_observer* obs = ssp_obs_init(settings);
    ssp_obs_dirs_create(obs);

    FILE* f = fopen(expected[2] ,"a");
    fclose(f);

    EXPECT_EQ(ssp_obs_dirs_traversal(obs), 0);

    size_t i, j = 0;
    for(i = 0; i < expected_count; i++) {
        for(j = 0; j < expected_count; j++) {
            std::string expected_filename = cut_fullname(expected[j]);
            std::string storage_filename = cut_fullname(ssp_is_char_ptr(settings.istorage)[i]);

            if (!storage_filename.compare(expected_filename)) {
                // printf("%s %s\n", (char *)storage_filename.data(), (char *)expected_filename.data());
                ASSERT_STREQ(storage_filename.data(), expected_filename.data());
                j = expected_count + 1;
                break;
            }
        }
        if (j != expected_count + 1) {
            FAIL() << ssp_is_char_ptr(settings.istorage)[i];
        }
    }

    remove(expected[2]);
    ssp_obs_destruct(obs);
}
