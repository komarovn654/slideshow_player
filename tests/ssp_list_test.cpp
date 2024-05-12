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
        list = ssp_list_init();
    }
    void TearDown()
    {
        ssp_list_destruct(list);
    }
};

TEST(TestList, ListInit) 
{
    ssp_list list = NULL;
    list = ssp_list_init();
    EXPECT_TRUE(list != NULL);

    EXPECT_STREQ(list->name, NULL);
    EXPECT_TRUE(list->next == NULL);
    EXPECT_EQ(ssp_ptr_storage_size(), 1);

    ssp_free(list->name);
    ssp_free(list);
}

TEST(TestList, ListDestruct) 
{
    ssp_list list = ssp_list_init();
    EXPECT_TRUE(list != NULL);

    ssp_list_destruct(list);
    EXPECT_EQ(ssp_ptr_storage_size(), 0);
}

TEST(TestList, ListDestruct_NullHead) 
{
    ssp_list_destruct(NULL);
    EXPECT_TRUE(true);

    ssp_list head = NULL;
    ssp_list_destruct(head);
    EXPECT_TRUE(true);
}

TEST(TestList, ListDestruct_MaxNodes) 
{
    const size_t len = MAX_PTR_STORAGE_LEN;
    char num[10];
    ssp_list list = ssp_list_init();
    
    for (size_t i = 1; i < len - 4; i++) {
        snprintf(num, sizeof(num), "%zu", i);
        ssp_list_insert(list, num);
    }
    EXPECT_EQ(ssp_ptr_storage_size(), ssp_ptr_storage_max_size());
    
    ssp_list_destruct(list);
    EXPECT_EQ(ssp_ptr_storage_size(), 0);
}

TEST_F(TestListFixture, ListHeadName_NULL) 
{
    EXPECT_TRUE(ssp_list_head_name(NULL) == NULL);
    EXPECT_TRUE(ssp_list_head_name(list) == NULL);
}

TEST_F(TestListFixture, ListHeadName_Insert) 
{
    ssp_list_insert(list, "item_0");
    EXPECT_STREQ(ssp_list_head_name(list), "item_0");

    ssp_list_insert(list, "item_1");
    EXPECT_STREQ(ssp_list_head_name(list), "item_0");
}

TEST_F(TestListFixture, ListInsert_Null) 
{
    ssp_list new_node = ssp_list_insert(NULL, "tail");
    EXPECT_TRUE(new_node == NULL);
    EXPECT_EQ(ssp_ptr_storage_size(), 1);

    new_node = ssp_list_insert(list, NULL);
    EXPECT_TRUE(new_node == NULL);
    EXPECT_EQ(ssp_ptr_storage_size(), 1);

    new_node = ssp_list_insert(NULL, "tail");
    EXPECT_TRUE(new_node == NULL);
    EXPECT_EQ(ssp_ptr_storage_size(), 1);
}

TEST_F(TestListFixture, ListInsert_1) 
{
    EXPECT_TRUE(ssp_list_insert(list, "tailtailtailtailtailtail") != NULL);

    EXPECT_EQ(ssp_ptr_storage_size(), 2);
    EXPECT_STREQ(list->name, "tailtailtailtailtailtail");
    EXPECT_TRUE(list->next == NULL);
}

TEST_F(TestListFixture, ListInsertv_1) 
{
    EXPECT_TRUE(ssp_list_insertv(list, "tailtailtailtailtailtail") != NULL);

    EXPECT_EQ(ssp_ptr_storage_size(), 2);
    EXPECT_STREQ(list->name, "tailtailtailtailtailtail");
    EXPECT_TRUE(list->next == NULL);
}

TEST_F(TestListFixture, ListInsert_Duplicate) 
{
    ssp_list duplicate = ssp_list_insert(list, "head");

    EXPECT_EQ(ssp_ptr_storage_size(), 2);
    EXPECT_EQ(list, duplicate);

    ssp_list mid = ssp_list_insert(list, "mid");
    ssp_list tail = ssp_list_insert(list, "tail");
    duplicate = ssp_list_insert(list, "mid");
    EXPECT_EQ(ssp_ptr_storage_size(), 6);
    EXPECT_EQ(mid, duplicate);

    duplicate = ssp_list_insert(list, "tail");
    EXPECT_EQ(ssp_ptr_storage_size(), 6);
    EXPECT_EQ(tail, duplicate);    
}

TEST_F(TestListFixture, ListInsert_MaxNodes)
{
    const size_t len = ssp_ptr_storage_max_size() / 2;
    char num[10];
    for (size_t i = 0; i < len; i++) {
        snprintf(num, sizeof(num), "%zu", i);
        if (i == len / 2) {
            snprintf(num, sizeof(num), "ssp");
        }
        if (ssp_list_insert(list, num) == NULL) {
            FAIL();
        }
    }

    EXPECT_EQ(ssp_ptr_storage_size(), ssp_ptr_storage_max_size());
    EXPECT_STREQ(list->name, "0");
    ssp_list node = list;
    for (size_t i = 1; i < len; i++) {
        snprintf(num, sizeof(num), "%zu", i);
        node = node->next;
        if (i == len / 2) {
            EXPECT_STREQ(node->name, "ssp");
            continue;
        }
        EXPECT_STREQ(node->name, num);
    }
}

TEST(TestList, ListRemove_RemoveSingleNodeList)
{
    ssp_list list = ssp_list_init();
    list = ssp_list_insert(list, "head");
    ssp_list_remove_node(&list, "head");
    EXPECT_EQ(ssp_ptr_storage_size(), 1);
    ssp_list_destruct(list);
}

TEST_F(TestListFixture, ListRemove_RemoveEmptyName)
{
    ssp_list_insert(list, "head");
    ssp_list_insert(list, "item_0");

    ssp_list_remove_node(&list, "");

    EXPECT_STREQ(list->name, "head");
    EXPECT_STREQ(list->next->name, "item_0");
}

TEST_F(TestListFixture, ListRemove_RemoveHead)
{
    ssp_list_insert(list, "head");
    ssp_list_insert(list, "mid");
    ssp_list_insert(list, "tail");
    ssp_list_remove_node(&list, "head");

    EXPECT_EQ(ssp_ptr_storage_size(), 4);
    EXPECT_STREQ(list->name, "mid");
    EXPECT_STREQ(list->next->name, "tail");
    EXPECT_TRUE(list->next->next == NULL);
}

TEST_F(TestListFixture, ListRemove_RemoveEmptyList)
{
    ssp_list_insert(list, "head");
    ssp_list_remove_node(NULL, "head");
    ssp_list_remove_node(&list, NULL);
    EXPECT_STREQ(list->name, "head");
}

TEST_F(TestListFixture, ListRemove_RemoveTail)
{
    ssp_list_insert(list, "head");
    ssp_list_insert(list, "mid");
    ssp_list_insert(list, "tail");
    ssp_list_remove_node(&list, "tail");
    
    EXPECT_EQ(ssp_ptr_storage_size(), 4);
    EXPECT_STREQ(list->name, "head");
    EXPECT_STREQ(list->next->name, "mid");
    EXPECT_TRUE(list->next->next == NULL);
}

TEST_F(TestListFixture, ListRemovev_RemoveTail)
{
    ssp_list_insert(list, "head");
    ssp_list_insert(list, "mid");
    ssp_list_insert(list, "tail");
    ssp_list_removev_node((void**)list, "tail");
    
    EXPECT_EQ(ssp_ptr_storage_size(), 4);
    EXPECT_STREQ(list->name, "head");
    EXPECT_STREQ(list->next->name, "mid");
    EXPECT_TRUE(list->next->next == NULL);
}

TEST_F(TestListFixture, ListRemove_RemoveMiddle)
{
    ssp_list_insert(list, "head");
    ssp_list_insert(list, "mid_0");
    ssp_list_insert(list, "mid_1");
    ssp_list_insert(list, "mid_2");
    ssp_list_insert(list, "tail");

    ssp_list_remove_node(&list, "mid_1");
    
    EXPECT_EQ(ssp_ptr_storage_size(), 8);
    EXPECT_STREQ(list->name, "head");
    EXPECT_STREQ(list->next->name, "mid_0");
    EXPECT_STREQ(list->next->next->name, "mid_2");
    EXPECT_STREQ(list->next->next->next->name, "tail");
    EXPECT_TRUE( list->next->next->next->next == NULL);

    ssp_list_remove_node(&list, "mid_0");
    
    EXPECT_EQ(ssp_ptr_storage_size(), 6);
    EXPECT_STREQ(list->name, "head");
    EXPECT_STREQ(list->next->name, "mid_2");
    EXPECT_STREQ(list->next->next->name, "tail");
    EXPECT_TRUE( list->next->next->next == NULL);
}

TEST_F(TestListFixture, ListTraversal_Errors)
{
    static char storage[7][10];

    EXPECT_EQ(ssp_list_traversal(list, NULL, 0), 1);
    EXPECT_EQ(ssp_list_traversal(NULL, (char **)storage, 0), 1);
}

TEST_F(TestListFixture, ListTraversal_LongName)
{
    static char storage[7][10] = {""};

    char items[8][20] = {"head", "very_long_filename", "mid_1", "mid_2", "mid_3",
        "mid_4", "tail"};

    for (size_t i = 0; i < 7; i++) {
        ssp_list_insert(list, items[i]);
    }

    EXPECT_EQ(ssp_list_traversal(list, (char **)storage, 10), 1);

    EXPECT_STREQ(items[0], storage[0]);
    EXPECT_STRNE(items[1], storage[1]);
    EXPECT_STRNE(items[2], storage[2]);
}

TEST_F(TestListFixture, ListTraversal)
{
    static char storage[7][10];

    char items[7][10] = {"head", "mid_0", "mid_1", "mid_2", "mid_3",
        "mid_4", "tail"};

    for (size_t i = 0; i < 7; i++) {
        ssp_list_insert(list, items[i]);
    }

    EXPECT_EQ(ssp_list_traversal(list, (char **)storage, 10), 0);

    for (size_t i = 0; i < 7; i++) {
        EXPECT_STREQ(items[i], storage[i]);
    }
}

TEST_F(TestListFixture, ListMoveHead)
{
    EXPECT_TRUE(ssp_list_move_head(list) == NULL);
    EXPECT_TRUE(ssp_list_head_name(list) == NULL);
    
    ssp_list_insert(list, "head");
    EXPECT_TRUE(ssp_list_move_head(list) == NULL);
    EXPECT_STREQ(ssp_list_head_name(list), "head");

    ssp_list_insert(list, "item");
    ssp_list new_head = ssp_list_move_head(list);
    EXPECT_TRUE(new_head != NULL);
    EXPECT_STREQ(ssp_list_head_name(list), "head");
    EXPECT_STREQ(ssp_list_head_name(new_head), "item");
}