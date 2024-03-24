#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "ssp_memory.h"

typedef struct ssp_node* ssp_list;

ssp_list ssp_list_init(const char* head_name);
void ssp_list_delete(ssp_list head);

ssp_list ssp_list_insert(ssp_list head, const char* tail_name);
void ssp_list_remove_node(ssp_list* head, const char* remove_name);
int ssp_list_print(ssp_list head, void (*print_func)(const char*));

#ifdef __cplusplus
}
#endif