#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "logman/logman.h"
#include "ssp_list.h"
#include "ssp_memory.h"
#include "ssp_helper.h"

#define delete_node(node) {\
    ssp_free((node)->name);\
    ssp_free(node);\
}\

typedef struct ssp_node ssp_node;

typedef struct ssp_node {
    char* name;
    ssp_node* next;
} ssp_node;

ssp_list ssp_list_init(void)
{
    ssp_node* head = (ssp_node*)ssp_malloc(sizeof(ssp_node));
    if (head == NULL) {
        log_error("The list wasn't initialized. Failed to allocate memory for node.");
        return NULL;
    }

    head->name = NULL;
    head->next = NULL;
    log_debug("List <%s> has been initialized", head->name);
    return head;
}

void ssp_list_destruct(ssp_list head)
{
    if (head == NULL || head == NULL) {
        log_warning("It's impossible to delete NULL list.");
        return;
    }

    while (head->next != NULL) {
        ssp_list node = head->next;
        delete_node(head);
        head = node;
    }

    delete_node(head);
    log_debug("List has been deleted");
}

char* ssp_list_head_name(ssp_list head)
{
    if (head == NULL || head->name == NULL) {
        return NULL;
    }

    return head->name;
}

ssp_list ssp_list_move_head(ssp_list head)
{
    if (head == NULL) {
        return NULL;
    }

    return head->next;
}

void ssp_list_remove_node(ssp_list *head, const char *remove_name)
{
    if (head == NULL || *head == NULL || (*head)->name == NULL || remove_name == NULL) {
        log_warning("It's impossible to delete NULL node(or from NULL list).");
        return;
    }

    if (strcmp((*head)->name, remove_name) == 0) {
        log_debug("Head has been deleted", (*head)->name);
        if ((*head)->next == NULL) {
            ssp_free((*head)->name);
            (*head)->name = NULL;
            return;
        }
        ssp_list node = (*head)->next;
        delete_node(*head);   
        (*head) = node;
        return;
    }

    ssp_list node = *head;
    while (node->next != NULL) {
        if (strcmp(node->next->name, remove_name) == 0) {
            log_debug("Node <%s> has been deleted", node->next->name);
            if (node->next->next == NULL) {
                delete_node(node->next);
                node->next = NULL;
                return;
            }
            ssp_list next_node = node->next->next;
            delete_node(node->next);
            node->next = next_node;
            return;
        }   
        node = node->next;
    }
}

void ssp_list_remove_nodev(void** head, const char* remove_name)
{
    ssp_list_remove_node((ssp_list*)&head, remove_name);
}

static int ssp_list_insert_name(ssp_list node, const char* name)
{
    size_t name_len = strlen(name);
    node->name = (char*)ssp_calloc(strlen(name) + 1, sizeof(char));
    if (node == NULL) {
        log_error("The new node wasn't created. Failed to allocate memory for name.");
        return 1;
    }
    memcpy(node->name, name, name_len);

    log_info("Node <%s> has been added", node->name);
    return 0;
}

ssp_list ssp_list_insert(ssp_list head, const char* tail_name)
{
    if (head == NULL || tail_name == NULL) {
        log_warning("It's impossible to insert NULL node(or in NULL list).");
        return NULL;
    }

    if (head->name == NULL) {
        if (ssp_list_insert_name(head, tail_name) != 0) {
            return NULL;
        }
        return head;
    }

    while (true && (head->name != NULL)) {
        if (strcmp(head->name, tail_name) == 0) {
            log_debug("Node <%s> already exists", head->name);
            return head;
        }

        if (head->next == NULL) {
            break;
        }
        head = head->next;
    }

    ssp_node* tail = (ssp_node*)ssp_malloc(sizeof(ssp_node));
    if (tail == NULL) {
        log_error("The new node wasn't created. Failed to allocate memory.");
        return NULL;
    }
    head->next = tail;

    if (ssp_list_insert_name(tail, tail_name) != 0) {
        return NULL;
    }

    tail->next = NULL;
    log_info("Node <%s> has been added", tail->name);
    return tail;
}

void* ssp_list_insertv(void* head, const char* tail_name)
{
    return ssp_list_insert(head, tail_name);
}

int ssp_list_traversal(ssp_list head, char **storage, size_t max_name_size)
{
    if (head == NULL || storage == NULL) {
        log_error("It's impossible to traversal the list. Args must not be NULL.");
        return -1;        
    }

    ssp_node* node = (ssp_node*)head;
    size_t i = 0;

    while (node != NULL && node->name != NULL) {
        if (snprintf((char *)storage + i * max_name_size, max_name_size, "%s", node->name) >= max_name_size) {
            log_error("Сan't save an item with that's too big name <%s>", node->name);
            return -1;
        };
        node = node->next;
        i++;
    }

    return i;
}

char* ssp_list_head_namev(void* head)
{
    return ssp_list_head_name(head);
}

void* ssp_list_move_headv(void* head)
{
    return ssp_list_move_head(head);
}

ssp_image_storage* ssp_list_init_is(void)
{
    ssp_image_storage* is = (ssp_image_storage*)ssp_malloc(sizeof(ssp_image_storage));

    is->storage_head = ssp_list_init();
    is->storage_ptr = is->storage_head;
    is->insert = ssp_list_insertv;
    is->remove = ssp_list_remove_nodev;
    is->image_name = ssp_list_head_namev;
    is->move_ptr_to_next = ssp_list_move_headv;

    return is;
}

void ssp_list_destruct_is(ssp_image_storage* is)
{
    ssp_list_destruct(is->storage_head);
    ssp_free(is);
}
