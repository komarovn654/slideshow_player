#pragma once

typedef struct ssp_node* list;

list ssp_list_init(const char* head_name);
void ssp_list_delete(list head);

char* ssp_list_get_name(list head, unsigned int num);
list ssp_list_insert(list head, const char* tail_name);
void ssp_list_remove(list head, const char* remove_name);
int ssp_list_print(list head);