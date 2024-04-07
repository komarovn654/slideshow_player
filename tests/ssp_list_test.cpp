#include <cstdlib>

#include "gtest/gtest.h"
#include "ssp_list.h"

typedef struct ssp_node {
    char* name;
    ssp_node* next;
} ssp_node;

class TestListFixture : public ::testing::Test
{
public:
    ssp_list list;
protected:
    void SetUp()
    {
        list = ssp_list_init("head");
    }
    void TearDown()
    {
        ssp_list_delete(list);
    }
};

TEST(TestList, ListInit) 
{
    ssp_list list = NULL;
    list = ssp_list_init("head");
    ASSERT_TRUE(list != NULL);

    ASSERT_STREQ(list->name, "head");
    ASSERT_TRUE(list->next == NULL);
    ASSERT_EQ(ssp_ptr_storage_size(), 2);

    ssp_free(list->name);
    ssp_free(list);
}

TEST(TestList, ListInit_NullHead) 
{
    ssp_list list = ssp_list_init(NULL);
    ASSERT_TRUE(list == NULL);
    ASSERT_EQ(ssp_ptr_storage_size(), 0);
}

TEST(TestList, ListDelete) 
{
    ssp_list list = ssp_list_init("head");
    ASSERT_TRUE(list != NULL);

    ssp_list_delete(list);
    ASSERT_EQ(ssp_ptr_storage_size(), 0);
}

TEST(TestList, ListDelete_NullHead) 
{
    ssp_list_delete(NULL);
    ASSERT_TRUE(true);

    ssp_list head = NULL;
    ssp_list_delete(head);
    ASSERT_TRUE(true);
}

TEST(TestList, ListDelete_MaxNodes) 
{
    const size_t len = MAX_PTR_STORAGE_LEN;
    char num[10];
    ssp_list list = ssp_list_init("head");
    ssp_list nodes[len] = {NULL};
    
    nodes[0] = list;
    for (size_t i = 1; i < len - 4; i++) {
        snprintf(num, sizeof(num), "%zu", i);
        nodes[i] = ssp_list_insert(list, num);
    }
    ASSERT_EQ(ssp_ptr_storage_size(), ssp_ptr_storage_max_size());
    
    ssp_list_delete(list);
    ASSERT_EQ(ssp_ptr_storage_size(), 0);
}

TEST_F(TestListFixture, ListInsert_Null) 
{
    ssp_list new_node = ssp_list_insert(NULL, "tail");
    ASSERT_TRUE(new_node == NULL);
    ASSERT_EQ(ssp_ptr_storage_size(), 2);

    new_node = ssp_list_insert(list, NULL);
    ASSERT_TRUE(new_node == NULL);
    ASSERT_EQ(ssp_ptr_storage_size(), 2);

    new_node = ssp_list_insert(NULL, "tail");
    ASSERT_TRUE(new_node == NULL);
    ASSERT_EQ(ssp_ptr_storage_size(), 2);
}

TEST_F(TestListFixture, ListInsert_1) 
{
    ASSERT_TRUE(ssp_list_insert(list, "tailtailtailtailtailtail") != NULL);

    ASSERT_EQ(ssp_ptr_storage_size(), 4);
    ASSERT_STREQ(list->name, "head");
    ASSERT_TRUE(list->next != NULL);
    printf("NAME: %s\n", list->next->name);
    ASSERT_STREQ(list->next->name, "tailtailtailtailtailtail");
}

TEST_F(TestListFixture, ListInsert_Duplicate) 
{
    ssp_list duplicate = ssp_list_insert(list, "head");

    ASSERT_EQ(ssp_ptr_storage_size(), 2);
    ASSERT_EQ(list, duplicate);

    ssp_list mid = ssp_list_insert(list, "mid");
    ssp_list tail = ssp_list_insert(list, "tail");
    duplicate = ssp_list_insert(list, "mid");
    ASSERT_EQ(ssp_ptr_storage_size(), 6);
    ASSERT_EQ(mid, duplicate);

    duplicate = ssp_list_insert(list, "tail");
    ASSERT_EQ(ssp_ptr_storage_size(), 6);
    ASSERT_EQ(tail, duplicate);    
}

TEST_F(TestListFixture, ListInsert_MaxNodes)
{
    const size_t len = ssp_ptr_storage_max_size() / 2;
    char num[10];
    for (size_t i = 0; i < len - 1; i++) {
        snprintf(num, sizeof(num), "%zu", i);
        if (i == len / 2) {
            snprintf(num, sizeof(num), "ssp");
        }
        if (ssp_list_insert(list, num) == NULL) {
            FAIL();
        }
    }

    ASSERT_EQ(ssp_ptr_storage_size(), ssp_ptr_storage_max_size());
    ASSERT_STREQ(list->name, "head");
    ssp_list node = list;
    for (size_t i = 0; i < len-1; i++) {
        snprintf(num, sizeof(num), "%zu", i);
        node = node->next;
        if (i == len / 2) {
            ASSERT_STREQ(node->name, "ssp");
            continue;
        }
        ASSERT_STREQ(node->name, num);
    }
}

TEST(TestList, ListRemove_RemoveSingleNodeList)
{
    ssp_list list = ssp_list_init("head");
    ssp_list_remove_node(&list, "head");
    ASSERT_EQ(ssp_ptr_storage_size(), 0);
}

TEST_F(TestListFixture, ListRemove_RemoveHead)
{
    ssp_list_insert(list, "mid");
    ssp_list_insert(list, "tail");
    ssp_list_remove_node(&list, "head");

    ASSERT_EQ(ssp_ptr_storage_size(), 4);
    ASSERT_STREQ(list->name, "mid");
    ASSERT_STREQ(list->next->name, "tail");
    ASSERT_TRUE(list->next->next == NULL);
}

TEST_F(TestListFixture, ListRemove_RemoveEmptyList)
{
    ssp_list_remove_node(NULL, "head");
    ssp_list_remove_node(&list, NULL);
    ASSERT_STREQ(list->name, "head");
}

TEST_F(TestListFixture, ListRemove_RemoveTail)
{
    ssp_list_insert(list, "mid");
    ssp_list_insert(list, "tail");
    ssp_list_remove_node(&list, "tail");
    
    ASSERT_EQ(ssp_ptr_storage_size(), 4);
    ASSERT_STREQ(list->name, "head");
    ASSERT_STREQ(list->next->name, "mid");
    ASSERT_TRUE(list->next->next == NULL);
}

TEST_F(TestListFixture, ListRemove_RemoveMiddle)
{
    ssp_list_insert(list, "mid_0");
    ssp_list_insert(list, "mid_1");
    ssp_list_insert(list, "mid_2");
    ssp_list_insert(list, "tail");
    ssp_list_remove_node(&list, "mid_1");
    
    ASSERT_EQ(ssp_ptr_storage_size(), 8);
    ASSERT_STREQ(list->name, "head");
    ASSERT_STREQ(list->next->name, "mid_0");
    ASSERT_STREQ(list->next->next->name, "mid_2");
    ASSERT_STREQ(list->next->next->next->name, "tail");
    ASSERT_TRUE( list->next->next->next->next == NULL);
}

static char names[7][10];

static void test_print_function(const char* name)
{
    static int i = 0;

    snprintf(names[i], 10, "%s", name);
    // printf("%s\n", name);
    i++;
}

TEST_F(TestListFixture, PrintList_Error)
{
    ASSERT_EQ(ssp_list_print(NULL, NULL), 1);

    char items[7][10] = {"head", "mid_0", "mid_1", "mid_2", "mid_3",
        "mid_4", "tail"};

    for (size_t i = 0; i < 7; i++) {
        ssp_list_insert(list, items[i]);
    }

    ssp_list_print(list, test_print_function);

    for (size_t i = 0; i < 7; i++) {
        ASSERT_STREQ(items[i], names[i]);
    }
}