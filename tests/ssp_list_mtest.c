#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <ssp_list.h>

#define INSERT_COMMAND ("add")
#define REMOVE_COMMAND ("del")

#define MAX_NUM_ITEMS (50)
#define MAX_ITEM_LEN  (128)

typedef enum {
    LC_UNKNOWN_COMMAND = 0,
    LC_INSERT,
    LC_REMOVE
} LIST_COMMAND;

char storage[MAX_NUM_ITEMS][MAX_ITEM_LEN];

static LIST_COMMAND parse_command(char* buf, char* result_item)
{
    char* command = strtok(buf, ":");
    if (command == NULL) {
        return LC_UNKNOWN_COMMAND;
    }

    char* item = strtok(NULL, ":");
    snprintf(result_item, MAX_ITEM_LEN, "%s", item);

    if (strcmp(command, INSERT_COMMAND) == 0 ) {
        return LC_INSERT;
    }

    if (strcmp(command, REMOVE_COMMAND) == 0 ) {
        return LC_REMOVE;
    }

    return LC_UNKNOWN_COMMAND;
}

int main(int argc, char* argv[])
{
    printf("Use commands add:item and del:item to insert and remove items from the list\n");
    char scan_buf[MAX_ITEM_LEN + 4];
    char result[MAX_ITEM_LEN];
    int size = 0;

    ssp_list list = ssp_list_init();

    while(1) {
        scanf("%s", scan_buf);

        switch (parse_command(scan_buf, result)) {
            case LC_INSERT:
                ssp_list_insert(list, result);
                break;
            case LC_REMOVE:
                ssp_list_remove_node(&list, result);
                break;
            case LC_UNKNOWN_COMMAND:
            default:
                printf("Unknown command\n");
                break;
        }

        if ((size = ssp_list_traversal(list, (char**)storage, MAX_ITEM_LEN)) == -1) {
            printf("List traversal error\n");
            return EXIT_FAILURE;
        }

        for (int i = 0; i < size; i++) {
            printf("%s --> ", storage[i]);
        }
        printf("NULL\n");
    }

    return EXIT_SUCCESS;
}
