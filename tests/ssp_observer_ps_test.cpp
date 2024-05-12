#include "gtest/gtest.h"

#include "ssp_memory.h"
#include "ssp_observer.h"
#include "ssp_observer_ps.h"

class TestObserverPSFixture : public ::testing::Test
{
public:
    static const size_t storage_size = 1024;
    char* storage;
    observer settings;

    static void* storage_insert(void* storage, const char* item_name)
    {
        return NULL;
    }

    static void  storage_remove(void** storage, const char* item_name)
    {
        return;
    }

    static bool filter(const char *file_name)
    {
        return false;
    }
protected:
    void SetUp()
    {
        storage = new char [storage_size];

        settings.dirs_count = SSP_OBS_DIRS_MAX_COUNT;
        settings.storage = storage;
        settings.storage_insert = storage_insert;
        settings.storage_remove = storage_remove;
        settings.filter = filter;
        for (size_t i = 0; i < SSP_OBS_DIRS_MAX_COUNT; i++) {
            settings.dirs[i] = new char [SSP_OBS_DIR_NAME_LEN];
            snprintf(settings.dirs[i], SSP_OBS_DIR_NAME_LEN, "../../tests/directory_%li", i);
        }
    }
    void TearDown()
    {
        for (size_t i = 0; i < SSP_OBS_DIRS_MAX_COUNT; i++) {
            rmdir(settings.dirs[i]);
        }

        delete(storage);

        for (size_t i = 0; i < SSP_OBS_DIRS_MAX_COUNT; i++) {
            delete(settings.dirs[i]);
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