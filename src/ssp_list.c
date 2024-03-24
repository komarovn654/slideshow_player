#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <logman/logman.h>

#include "ssp_list.h"

#define delete_node(node) {\
    ssp_free((node)->name);\
    ssp_free(node);\
}\

typedef struct ssp_node ssp_node;

typedef struct ssp_node {
    char* name;
    ssp_node* next;
} ssp_node;

ssp_list ssp_list_init(const char* head_name)
{
    if (head_name == NULL) {
        log_warning("The list wasn't initialized. A non-NULL name is expected.");
        return NULL;
    }

    ssp_node* head = (ssp_node*)ssp_malloc(sizeof(ssp_node));
    if (head == NULL) {
        log_error("The list wasn't initialized. Failed to allocate memory for node.");
        return NULL;
    }

    size_t name_len = strlen(head_name);
    head->name = (char*)ssp_malloc(name_len);
    if (head->name == NULL) {
        log_error("The list wasn't initialized. Failed to allocate memory for name.");
        return NULL;
    }
    memcpy(head->name, head_name, name_len);

    head->next = NULL;
    log_debug("List <%s> has been initialized", head->name);
    return head;
}

void ssp_list_delete(ssp_list head)
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

void ssp_list_remove_node(ssp_list* head, const char* remove_name)
{
    if (head == NULL || *head == NULL || remove_name == NULL) {
        log_warning("It's impossible to delete NULL node(or from NULL list).");
        return;
    }

    if (strcmp((*head)->name, remove_name) == 0) {
        log_debug("Node <%s> has been deleted", (*head)->name);
        if ((*head)->next == NULL) {
            delete_node(*head);
            return;
        }
        ssp_list node = (*head)->next;
        delete_node(*head);   
        (*head) = node;
        return;
    }

    ssp_list node = (*head)->next;
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

ssp_list ssp_list_insert(ssp_list head, const char* tail_name)
{
    if (head == NULL || tail_name == NULL) {
        log_warning("It's impossible to insert NULL node(or in NULL list).");
        return NULL;
    }

    while (true) {
        if (strcmp(head->name, tail_name) == 0) {
            log_warning("Node <%s> already exists", head->name);
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

    tail->name = (char*)ssp_malloc(sizeof(tail_name));
    if (tail == NULL) {
        log_error("The new node wasn't created. Failed to allocate memory for name.");
        return NULL;
    }
    memcpy(tail->name, tail_name, sizeof(&tail_name));

    tail->next = NULL;
    log_debug("Node <%s> has been added", tail->name);
    return tail;
}

int ssp_list_print(ssp_list head, void (*print_func)(const char*))
{
    if (head == NULL || print_func == NULL) {
        log_error("it's impossible to print the list. Args must not be NULL.");
        return 1;        
    }

    ssp_node* node = (ssp_node*)head;

    while (node != NULL && node->name != NULL) {
        print_func(node->name);
        node = node->next;
    }

    return 0;
}

// char* ssp_list_get_name(ssp_list head, unsigned int num)
// {
//     unsigned int i;
//     ssp_node* node = (ssp_node*)head;

//     for (i = 0; node != NULL && i != num; i++) {
//         node = (ssp_node*)node->next;
//     }
    
//     if (i != num || node == NULL) {
//         return NULL;
//     }

//     return node->name;
// }

