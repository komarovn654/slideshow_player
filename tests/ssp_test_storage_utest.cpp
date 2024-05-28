#include <string.h>

#include "gtest/gtest.h"
#include "ssp_test_storage.h"
#include "ssp_helper.h"
#include "ssp_memory.h"

extern "C" {
    ssp_static void* ssp_test_storage_insert(void* storage, const char* item_name);
    ssp_static void ssp_test_storage_remove(void** storage, const char* item_name);
    ssp_static char* ssp_test_storage_item_name(void* storage);
    ssp_static void* ssp_test_storage_move_ptr(void* storage);
}

class TestStorageFixture : public ::testing::Test
{
public:
    ssp_image_storage* storage;
protected:
    void SetUp()
    {
        storage = ssp_test_storage_init();
    }
    void TearDown()
    {
        ssp_test_storage_destruct(storage);
    }
};

TEST(TestStorage, TestStorageInit) 
{
    ssp_image_storage* storage = ssp_test_storage_init();

    ASSERT_TRUE(storage != NULL);
    ASSERT_EQ(storage->insert, ssp_test_storage_insert);
    ASSERT_EQ(storage->remove, ssp_test_storage_remove);
    ASSERT_EQ(storage->move_ptr_to_next, ssp_test_storage_move_ptr);
    ASSERT_EQ(storage->image_name, ssp_test_storage_item_name);

    ASSERT_EQ(ssp_ptr_storage_size(), SSP_TS_MAX_ITEM_COUNT + 2);
    ssp_test_storage_destruct(storage);
}

TEST(TestStorage, TestStorageDestruct) 
{
    ssp_image_storage* storage = ssp_test_storage_init();

    ASSERT_EQ(ssp_ptr_storage_size(), SSP_TS_MAX_ITEM_COUNT + 2);
    ssp_test_storage_destruct(storage);
    ASSERT_EQ(ssp_ptr_storage_size(), 0);
}

TEST_F(TestStorageFixture, TestStorageInsert) 
{
    ASSERT_STREQ((char*)storage->insert(storage->storage_ptr, "first_item"), "first_item");
    ASSERT_STREQ((char*)storage->insert(storage->storage_ptr, "second_item"), "second_item");

    ASSERT_STREQ(((char**)storage->storage_ptr)[0], "first_item");
    ASSERT_STREQ(((char**)storage->storage_ptr)[1], "second_item");
}

TEST_F(TestStorageFixture, TestStorageInsert_LongName) 
{
    char long_name[SSP_TS_ITEM_LEN];
    memset(long_name, 'a', SSP_TS_ITEM_LEN);

    ASSERT_TRUE(storage->insert(storage->storage_ptr, long_name) == NULL);
    ASSERT_STREQ(((char**)storage->storage_ptr)[0], "");
}

TEST_F(TestStorageFixture, TestStorageInsert_Overflow) 
{
    for (size_t i = 0; i < SSP_TS_MAX_ITEM_COUNT; i++) {
        storage->insert(storage->storage_ptr, "item");
    }

    ASSERT_TRUE(storage->insert(storage->storage_ptr, "item") == NULL);
}

TEST_F(TestStorageFixture, TestStorageRemove) 
{
    storage->insert(storage->storage_ptr, "first_item");
    storage->insert(storage->storage_ptr, "second_item");

    storage->remove((void**)(storage->storage_ptr), "first_item");
    ASSERT_STREQ(((char**)storage->storage_ptr)[0], "");
    storage->remove((void**)(storage->storage_ptr), "second_item");
    ASSERT_STREQ(((char**)storage->storage_ptr)[1], "");
}

TEST_F(TestStorageFixture, TestStorageHeadName) 
{
    storage->insert(storage->storage_ptr, "first_item");
    ASSERT_STREQ(((char*)storage->image_name(storage->storage_ptr)), "first_item");

    storage->insert(storage->storage_ptr, "second_item");
    ASSERT_STREQ(((char*)storage->image_name(storage->storage_ptr)), "first_item");
}

TEST_F(TestStorageFixture, TestStorageMoveHead) 
{
    ASSERT_EQ(storage->move_ptr_to_next(storage->storage_ptr), ((char**)storage->storage_ptr)[1]);
}
