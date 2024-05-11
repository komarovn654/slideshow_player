#include <stdio.h>

#include "ssp_list.h"

int main(void)
{
    list image_list = ssp_list_init("hello");

    ssp_list_insert(image_list, "second");
    ssp_list_insert(image_list, "third");
    ssp_list_insert(image_list, "fourth");
    ssp_list_insert(image_list, "fifth");
    ssp_list_insert(image_list, "sixth");

    ssp_list_remove(image_list, "second");
    ssp_list_remove(image_list, "third");
    ssp_list_remove(image_list, "fourth");
    ssp_list_remove(image_list, "fifth");
    ssp_list_remove(image_list, "sixth");
    ssp_list_remove(image_list, "fih");
    ssp_list_remove(image_list, "hello");

    ssp_list_print(image_list);

    ssp_list_destruct(image_list);
}