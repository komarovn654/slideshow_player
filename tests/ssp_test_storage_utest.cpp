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
    ssp_static void ssp_test_storage_destruct(ssp_test_storage* storage);
    ssp_static ssp_test_storage* ssp_test_storage_init(void);
}

class TestStorageFixture : public ::testing::Test
{
public:
    ssp_test_storage* storage;
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
    ssp_test_storage* storage = ssp_test_storage_init();
    ASSERT_TRUE(storage != NULL);
    ASSERT_EQ(storage->len, 0);
    ASSERT_EQ(storage->position, 0);
    ASSERT_EQ(storage->cap, SSP_TS_MAX_ITEM_COUNT);

    ssp_test_storage_destruct(storage);
}

TEST(TestStorage, TestStorageDestruct) 
{
    ssp_test_storage* storage = ssp_test_storage_init();
    ASSERT_EQ(ssp_ptr_storage_size(), SSP_TS_MAX_ITEM_COUNT + 2);
    
    ssp_test_storage_destruct(storage);
    ASSERT_EQ(ssp_ptr_storage_size(), 0);
}

TEST_F(TestStorageFixture, TestStorageInsert) 
{
    ASSERT_STREQ((char*)ssp_test_storage_insert(storage, "first_item"), "first_item");
    ASSERT_STREQ((char*)ssp_test_storage_insert(storage, "second_item"), "second_item");
    ASSERT_EQ(storage->len, 2);

    ASSERT_STREQ(ssp_ts_char_ptr(storage)[0], "first_item");
    ASSERT_STREQ(ssp_ts_char_ptr(storage)[1], "second_item");
}

TEST_F(TestStorageFixture, TestStorageInsert_LongName) 
{
    char long_name[SSP_FULL_NAME_MAX_LEN];
    memset(long_name, 'a', SSP_FULL_NAME_MAX_LEN);

    ASSERT_TRUE(ssp_test_storage_insert(storage, long_name) == NULL);
    ASSERT_EQ(storage->len, 0);
}

TEST_F(TestStorageFixture, TestStorageInsert_Overflow) 
{
    for (size_t i = 0; i < storage->cap; i++) {
        ASSERT_STREQ((char*)ssp_test_storage_insert(storage, "item"), "item");
    }

    ASSERT_TRUE(ssp_test_storage_insert(storage, "item") == NULL);
}

TEST_F(TestStorageFixture, TestStorageRemove) 
{
    ssp_test_storage_insert(storage, "first_item");
    ssp_test_storage_insert(storage, "second_item");

    ssp_test_storage_remove((void**)&storage, "first_item");
    ASSERT_STREQ(ssp_ts_char_ptr(storage)[0], "second_item");
    ASSERT_STREQ(ssp_ts_char_ptr(storage)[1], "");
    ASSERT_EQ(storage->len, 1);

    ssp_test_storage_remove((void**)&storage, "second_item");
    ASSERT_STREQ(ssp_ts_char_ptr(storage)[0], "");
    ASSERT_STREQ(ssp_ts_char_ptr(storage)[1], "");
    ASSERT_EQ(storage->len, 0);
}

TEST_F(TestStorageFixture, TestStorageHeadName) 
{
    ssp_test_storage_insert(storage, "first_item");
    ssp_test_storage_insert(storage, "second_item");

    ASSERT_STREQ(ssp_test_storage_item_name(storage), "first_item");
}

TEST(TestStorage, TestStorageInitImageStorage) 
{
    ssp_image_storage* is = ssp_test_storage_init_is();

    ASSERT_EQ(is->insert, ssp_test_storage_insert);
    ASSERT_EQ(is->remove, ssp_test_storage_remove);
    ASSERT_EQ(is->move_to_next, ssp_test_storage_move_ptr);
    ASSERT_EQ(is->image_name, ssp_test_storage_item_name);

    ssp_test_storage_destruct_is(is);
}

TEST(TestStorage, TestStorageDestructImageStorage) 
{
    ssp_image_storage* is = ssp_test_storage_init_is();
    ASSERT_EQ(ssp_ptr_storage_size(), 23);

    ssp_test_storage_destruct_is(is);
    ASSERT_EQ(ssp_ptr_storage_size(), 0);
}
