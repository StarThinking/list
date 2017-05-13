#include <stdio.h>

#include "src/list.h"

static int item_equal(void *a, void *b) {
    long *a1 = (long*) a;
    long *b1 = (long*) b;
    return *a1 == *b1;
}

int main(void) {
    list_t *list = list_new();
    list->match = item_equal;

    long epoch = 1234567;
    list_rpush(list, list_node_new(&epoch));

    long epoch2 = epoch;
    
    list_node_t *item = list_find(list, &epoch2);
    if(item == NULL)
        printf("cannot find item\n");

    printf("list len = %d\n", list->len);

    list_remove(list, item);
    printf("after removal list len = %d\n", list->len);

    list_destroy(list);

    return 1;
}
