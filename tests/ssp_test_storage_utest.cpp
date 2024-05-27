#include "gtest/gtest.h"
#include "ssp_test_storage.h"
#include "ssp_helper.h"
#include "ssp_memory.h"

class TestStorageFixture : public ::testing::Test
{
public:
    ssp_image_storage* storage;
protected:
    void SetUp()
    {
        storage = ssp_test_storage_init(10);
    }
    void TearDown()
    {
    }
};

TEST_F(TestStorageFixture, TestStorageInit) 
{
    // ssp_image_storage* storage = ssp_test_storage_init(10);
    ASSERT_TRUE(storage != NULL);
    // ASSERT_EQ(ssp_ptr_storage_size(), 12);
    printf("sda\n");
    snprintf(((char**)storage->storage_head)[0], SSP_TEST_STORAGE_ITEM_LEN, "first item");
    printf("%s\n", ((char**)storage->storage_head)[0]);
    snprintf(((char**)storage->storage_head)[1], SSP_TEST_STORAGE_ITEM_LEN, "second item");
    printf("%s\n", ((char**)storage->storage_head)[1]);
    snprintf(((char**)storage->storage_head)[2], SSP_TEST_STORAGE_ITEM_LEN, "third item");
    printf("%s\n", ((char**)storage->storage_head)[2]);
}
