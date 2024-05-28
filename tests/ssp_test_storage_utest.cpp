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
    const size_t items_count = 10;
    ssp_image_storage* storage;
protected:
    void SetUp()
    {
        storage = ssp_test_storage_init(items_count);
    }
    void TearDown()
    {
        ssp_test_storage_destruct(storage);
    }
};

TEST(TestStorage, TestStorageInit) 
{
    const size_t items_count = 10;
    ssp_image_storage* storage = ssp_test_storage_init(items_count);

    ASSERT_TRUE(storage != NULL);
    ASSERT_EQ(storage->items_count, items_count);
    ASSERT_EQ(storage->insert, ssp_test_storage_insert);
    ASSERT_EQ(storage->remove, ssp_test_storage_remove);
    ASSERT_EQ(storage->move_ptr_to_next, ssp_test_storage_move_ptr);
    ASSERT_EQ(storage->image_name, ssp_test_storage_item_name);

    ASSERT_EQ(ssp_ptr_storage_size(), items_count + 2);
    ssp_test_storage_destruct(storage);
}

TEST(TestStorage, TestStorageDestruct) 
{
    const size_t items_count = 10;
    ssp_image_storage* storage = ssp_test_storage_init(items_count);

    ASSERT_EQ(ssp_ptr_storage_size(), items_count + 2);
    ssp_test_storage_destruct(storage);
    ASSERT_EQ(ssp_ptr_storage_size(), 0);
}

TEST_F(TestStorageFixture, TestStorageInsert) 
{
    const size_t items_count = 10;
    ssp_image_storage* storage = ssp_test_storage_init(items_count);

    ASSERT_EQ(ssp_ptr_storage_size(), items_count + 2);
    ssp_test_storage_destruct(storage);
    ASSERT_EQ(ssp_ptr_storage_size(), 0);
}
