#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ssp_list.h"

#define assert_node(node) if (node == NULL) return;

typedef struct ssp_node ssp_node;

typedef struct ssp_node {
    char* name;
    ssp_node* next;
} ssp_node;

static void ssp_node_delete(ssp_node* node);

list ssp_list_init(const char* head_name)
{
    ssp_node* head = (ssp_node*)malloc(sizeof(ssp_node));

    head->name = (char*)malloc(sizeof(head_name));
    memcpy(head->name, head_name, sizeof(head_name));

    head->next = NULL;
    return (list)head;
}

list ssp_list_insert(list head, const char* tail_name)
{
    ssp_node* node = (ssp_node*)head;

    while (node->next != NULL) {
        node = (ssp_node*)node->next;
    }

    ssp_node* tail = (ssp_node*)malloc(sizeof(ssp_node));
    node->next = tail;

    tail->name = (char*)malloc(sizeof(tail_name));
    memcpy(tail->name, tail_name, sizeof(tail_name));

    tail->next = NULL;
    return (list)tail;
}

void ssp_list_remove(list head, const char* remove_name)
{
    ssp_node* node = (ssp_node*)head;

    if (strcmp(node->name, remove_name) == 0) {
        ssp_node_delete(node);
        return;
    }

    while (node->next != NULL) {
        ssp_node* next_node = node->next;
        if (strcmp(next_node->name, remove_name) == 0) {
            node->next = next_node->next;
            ssp_node_delete(next_node);
            return;
        }   
        node = node->next;
    }

    // ssp_node* tail = (ssp_node*)malloc(sizeof(ssp_node));
    // node->next = (p_node)tail;

    // tail->name = (char*)malloc(sizeof(tail_name));
    // memcpy(tail->name, tail_name, sizeof(tail_name));

    // tail->next = NULL;
    // return (list)tail;
}

int ssp_list_print(list head)
{
    ssp_node* node = (ssp_node*)head;

    while (node != NULL && node->name != NULL) {
        printf("%s\n", node->name);
        node = node->next;
    }
}

char* ssp_list_get_name(list head, unsigned int num)
{
    unsigned int i;
    ssp_node* node = (ssp_node*)head;

    for (i = 0; node != NULL && i != num; i++) {
        node = (ssp_node*)node->next;
    }
    
    if (i != num || node == NULL) {
        return NULL;
    }

    return node->name;
}

void ssp_list_delete(list head)
{
    ssp_node* node = (ssp_node*)head;

    while (node != NULL) {
        ssp_node* prev = node;
        node = (ssp_node*)node->next;
        free(prev->name);
        free(prev);
        prev->name = NULL;
        prev = NULL;
    }
}

static void ssp_node_delete(ssp_node* node)
{
    printf("%s: %p %p\n", node->name, &node->name, node->next);
    free(node->name);
    free(node);
    node->name = NULL;
    node = NULL;
}