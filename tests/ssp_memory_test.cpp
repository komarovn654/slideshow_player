#include <cstdlib>

#include "gtest/gtest.h"
#include "ssp_memory.h"

extern "C" {
    extern void **ssp_ptr_storage();
    extern void ssp_reset_ptr_storage_size();
}

class TestMemory : public ::testing::Test
{
protected:
    void SetUp()
    {
        void **storage_ptr = ssp_ptr_storage();
        for (size_t i = 0; i < MAX_PTR_STORAGE_LEN; i++) {
            *storage_ptr++ = NULL;
        }
        ssp_reset_ptr_storage_size();
    }
    void TearDown()
    {
        void **storage_ptr = ssp_ptr_storage();
        for (size_t i = 0; i < MAX_PTR_STORAGE_LEN; i++) {
            free(*storage_ptr++);
        }
    }
};

TEST_F(TestMemory, Malloc) 
{
    void *ptr = ssp_malloc(1);
    ASSERT_TRUE(ptr != NULL);
}

TEST_F(TestMemory, Free) 
{
    void *ptr = ssp_malloc(1);
    ssp_free(ptr);
    ssp_free(NULL);

    ASSERT_TRUE(true); // HardFault check
}

TEST_F(TestMemory, PtrStorage_Empty) 
{
    void **storage_ptr = ssp_ptr_storage();
    for (size_t i = 0; i < MAX_PTR_STORAGE_LEN; i++) {
        if (*storage_ptr++ != NULL) {
            ASSERT_TRUE(*storage_ptr == NULL);
        }
    }
}

TEST_F(TestMemory, StorePointer_1) 
{
    void *ptr = ssp_malloc(1);
    ASSERT_TRUE(ssp_ptr_storage()[0] == ptr);
    ASSERT_EQ(ssp_ptr_storage_size(), 1);
}

TEST_F(TestMemory, StorePointer_Max) 
{
    void *test_storage[MAX_PTR_STORAGE_LEN] = { NULL };

    for (size_t i = 0; i < MAX_PTR_STORAGE_LEN; i++) {
        void *ptr = ssp_malloc(1);
        test_storage[i] = ptr;
    }

    void **storage_ptr = ssp_ptr_storage();
    for (size_t i = 0; i < MAX_PTR_STORAGE_LEN; i++) {
        if (*storage_ptr++ != test_storage[i]) {
            ASSERT_TRUE(*storage_ptr == test_storage[i]);
        }        
    }
    ASSERT_EQ(ssp_ptr_storage_size(), MAX_PTR_STORAGE_LEN);
}

TEST_F(TestMemory, StorePointer_Overflow) 
{
    for (size_t i = 0; i < MAX_PTR_STORAGE_LEN; i++) {
        ssp_malloc(1);
    }

    void *ptr = ssp_malloc(1);
    ASSERT_TRUE(ptr == NULL);
}

TEST_F(TestMemory, DeletePointer_1) 
{
    void **storage_ptr = ssp_ptr_storage();

    void *ptr = ssp_malloc(1);
    ASSERT_TRUE((ptr == storage_ptr[0]) && (ptr != NULL));
    ASSERT_EQ(ssp_ptr_storage_size(), 1);
    
    ssp_free(ptr);
    ASSERT_EQ(ssp_ptr_storage_size(), 0);
    ASSERT_TRUE(*storage_ptr == NULL);
}

TEST_F(TestMemory, DeletePointer_Max) 
{
    void *test_storage[MAX_PTR_STORAGE_LEN] = { NULL };
    for (size_t i = 0; i < MAX_PTR_STORAGE_LEN; i++) {
        void *ptr = ssp_malloc(1);
        test_storage[i] = ptr;
    }
    ASSERT_EQ(ssp_ptr_storage_size(), MAX_PTR_STORAGE_LEN);

    for (size_t i = 0; i < MAX_PTR_STORAGE_LEN; i++) {
        ssp_free(test_storage[i]);
    }
    
    void **storage_ptr = ssp_ptr_storage();
    for (size_t i = 0; i < MAX_PTR_STORAGE_LEN; i++) {
        if (*storage_ptr++ != NULL) {
            ASSERT_TRUE(*storage_ptr == NULL);
        }
    }
    ASSERT_EQ(ssp_ptr_storage_size(), 0);
}

TEST_F(TestMemory, PtrStorageSize) 
{
    ASSERT_EQ(ssp_ptr_storage_size(), 0);

    ssp_malloc(1);
    ASSERT_EQ(ssp_ptr_storage_size(), 1);

    for (size_t i = 1; i < MAX_PTR_STORAGE_LEN; i++) {
        ssp_malloc(1);
        if (ssp_ptr_storage_size() != i + 1) {
            ASSERT_EQ(ssp_ptr_storage_size(), i + 1);
        }
    }

    void **ptr = ssp_ptr_storage();
    for (size_t i = 0; i < MAX_PTR_STORAGE_LEN; i++) {
        ssp_free(*ptr);
        if (ssp_ptr_storage_size() != MAX_PTR_STORAGE_LEN - 1 - i) {
            ASSERT_EQ(ssp_ptr_storage_size(), MAX_PTR_STORAGE_LEN - 1 - i);
        }
    }
}

TEST_F(TestMemory, MaxPtrStorageSize) 
{
    ASSERT_EQ(ssp_ptr_storage_max_size(), MAX_PTR_STORAGE_LEN);
}