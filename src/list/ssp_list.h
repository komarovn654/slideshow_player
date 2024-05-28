#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "ssp_helper.h"

typedef struct ssp_node* ssp_list;

ssp_list ssp_list_init(void);
char* ssp_list_head_name(ssp_list head);
void ssp_list_destruct(ssp_list head);
ssp_list ssp_list_move_head(ssp_list head);
ssp_list ssp_list_insert(ssp_list head, const char* tail_name);
void ssp_list_remove_node(ssp_list* head, const char* remove_name);
int ssp_list_traversal(ssp_list head, char** storage, size_t max_name_size);

char* ssp_list_head_namev(void* head);
void* ssp_list_move_headv(void* head);
void* ssp_list_insertv(void* head, const char* tail_name);
void ssp_list_remove_nodev(void** head, const char* remove_name);

ssp_image_storage* ssp_list_init_is(void);
void ssp_list_destruct_is(ssp_image_storage* is);

#ifdef __cplusplus
}
#endif
