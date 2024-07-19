#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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
        ssp_syslog(LOG_CRIT, "<List>: <Initialization>: The list wasn't initialized. Failed to allocate memory for the node");
        return NULL;
    }

    head->name = NULL;
    head->next = NULL;
    ssp_syslog(LOG_INFO, "<List>: <Initialization>: List was initialized");
    return head;
}

void ssp_list_destruct(ssp_list head)
{
    if (head == NULL || head == NULL) {
        ssp_syslog(LOG_WARNING, "<List>: <Destruction>: Unable to delete a NULL list");
        return;
    }

    while (head->next != NULL) {
        ssp_list node = head->next;
        delete_node(head);
        head = node;
    }

    delete_node(head);
    ssp_syslog(LOG_INFO, "SSP List. List was destructed");
}

char* ssp_list_head_name(ssp_list head)
{
    if (head == NULL || head->name == NULL) {
        ssp_syslog(LOG_WARNING, "<List>: <Head name>: Unable to get name from a NULL node");
        return NULL;
    }

    return head->name;
}

ssp_list ssp_list_move_head(ssp_list head)
{
    if (head == NULL) {
        ssp_syslog(LOG_WARNING, "<List>: <Move head>: NULL-head list");
        return NULL;
    }

    return head->next;
}

void ssp_list_remove_node(ssp_list *head, const char *remove_name)
{
    if (remove_name == NULL) {
        ssp_syslog(LOG_WARNING, "<List>: <Remove node>: Unable to remove a NULL node");
        return;
    }

    if ((head == NULL) || (*head == NULL) || ((*head)->name == NULL)) {
        ssp_syslog(LOG_WARNING, "<List>: <Remove node>: List wasn't initialized; %s wasn't removed", remove_name);
        return;
    }

    if (strcmp((*head)->name, remove_name) == 0) {
        ssp_syslog(LOG_DEBUG, "<List>: <Remove node>: Head <%s> was deleted", (*head)->name);
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
            ssp_syslog(LOG_DEBUG, "<List>: <Remove node>: Node <%s> was deleted", node->next->name);
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
    ssp_list_remove_node((ssp_list*)head, remove_name);
}

ssp_static int ssp_list_insert_name(ssp_list node, const char* name)
{
    size_t name_len = strlen(name);
    node->name = (char*)ssp_calloc(strlen(name) + 1, sizeof(char));
    if (node == NULL) {
        ssp_syslog(LOG_ERR, "<List>: <Insert node>: Node <%s> wasn't created. Failed to allocate memory for the name", name);
        return 1;
    }
    memcpy(node->name, name, name_len);

    ssp_syslog(LOG_DEBUG, "<List>: <Insert node>: Node <%s> was added", node->name);
    return 0;
}

ssp_list ssp_list_insert(ssp_list head, const char* tail_name)
{
    if (tail_name == NULL) {
        ssp_syslog(LOG_WARNING, "<List>: <Insert node>: Unable to insert a NULL node");
        return NULL;
    }

    if (head == NULL) {
        ssp_syslog(LOG_WARNING, "<List>: <Insert node>: List wasn't initialized; %s wasn't inserted", tail_name);
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
            ssp_syslog(LOG_DEBUG, "<List>: <Insert node>: Node <%s> already exists", head->name);
            return head;
        }

        if (head->next == NULL) {
            break;
        }
        head = head->next;
    }

    ssp_node* tail = (ssp_node*)ssp_malloc(sizeof(ssp_node));
    if (tail == NULL) {
        ssp_syslog(LOG_ERR, "<List>: <Insert node>: Node <%s> wasn't created. Failed to allocate memory for the node", tail_name);
        return NULL;
    }
    head->next = tail;

    if (ssp_list_insert_name(tail, tail_name) != 0) {
        return NULL;
    }

    tail->next = NULL;
    return tail;
}

void* ssp_list_insertv(void* head, const char* tail_name)
{
    return ssp_list_insert(head, tail_name);
}

int ssp_list_traversal(ssp_list head, char **storage, size_t max_name_size)
{
    if (head == NULL || storage == NULL) {
        ssp_syslog(LOG_WARNING, "<List>: <List traversal>: Unable to traversal the list. Args must not be NULL");
        return -1;        
    }

    ssp_node* node = (ssp_node*)head;
    size_t i = 0;

    while (node != NULL && node->name != NULL) {
        if (snprintf((char *)storage + i * max_name_size, max_name_size, "%s", node->name) >= max_name_size) {
            ssp_syslog(LOG_WARNING, "<List>: <List traversal>: Сan't save an item with that's too big name <%s>", node->name);
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

    is->storage = ssp_list_init();
    is->insert = ssp_list_insertv;
    is->remove = ssp_list_remove_nodev;
    is->image_name = ssp_list_head_namev;
    is->move_to_next = ssp_list_move_headv;

    ssp_syslog(LOG_INFO, "<List>: <Initialization>: List was initialized as an image storage");
    return is;
}

void ssp_list_destruct_is(ssp_image_storage* is)
{
    ssp_list_destruct(is->storage);
    ssp_free(is);
    ssp_syslog(LOG_INFO, "<List>: <Initialization>: Image storage(List) was destructed");
}
