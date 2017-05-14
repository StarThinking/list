#include <stdio.h>
#include <string.h>

#include "hashtable.h"
#include "src/list.h"

static int ip_equal(void *a, void *b) {
    return 0 == strcmp(a, b);
}

int hashtable_test(void) {
    printf("hashtable_test\n");
    hash_table epoch_list_ht;
    ht_init(&epoch_list_ht, HT_NONE, 0.05);

    long epoch_id = 1;
    char *saddr = "10.0.0.13";
    size_t value_size;

    list_t **ip_list;
    ip_list = (list_t**) ht_get(&epoch_list_ht, &epoch_id, sizeof(long), &value_size);
    
    if(ip_list == NULL) {
        list_t *tmp = list_new();
        tmp->match = ip_equal;
        ip_list = &tmp;
        ht_insert(&epoch_list_ht, &epoch_id, sizeof(long), ip_list, sizeof(list_t*));
    }
    
    if(list_find(*ip_list, saddr) == NULL)
        list_rpush(*ip_list, list_node_new(saddr));
    printf("ip_list size = %d\n", (*ip_list)->len);

    long epoch_id1 = 1;
    char *saddr1 = "10.0.0.13";

    ip_list = (list_t**) ht_get(&epoch_list_ht, &epoch_id1, sizeof(long), &value_size);
    if(ip_list != NULL) {
        list_node_t *ip_to_remove = list_find(*ip_list, saddr1);
        if(ip_to_remove != NULL)
            list_remove(*ip_list, ip_to_remove);
    }
    printf("after remove\n");

    ip_list = (list_t**) ht_get(&epoch_list_ht, &epoch_id1, sizeof(long), &value_size);
    if(ip_list != NULL) {
        printf("ip_list size = %d\n", (*ip_list)->len);
        list_iterator_t *it = list_iterator_new(*ip_list, LIST_HEAD);
        list_node_t *next = list_iterator_next(it);
        while(next != NULL) {
            printf("no NULL\n");
            next = list_iterator_next(it);
            printf("Ip %s expired at Epoch %ld\n", (char*)next->val, epoch_id1);
            list_remove(*ip_list, next);
        }
        list_iterator_destroy(it);
        printf("here\n");
        list_destroy(*ip_list);
        ht_remove(&epoch_list_ht, &epoch_id1, sizeof(long));
        printf("here1\n");
        printf("here2\n");
    }

    ht_destroy(&epoch_list_ht);
}

int ip_test(void) {
    printf("ip_test\n");
    list_t *list = list_new();
    list->match = ip_equal;

    char *node = "10.0.0.13";
    list_rpush(list, list_node_new(node));

    if(!list_find(list, "10.0.0.13"))
        list_rpush(list, list_node_new(node));
    printf("list len = %d\n", list->len);
    
    char *node2 = malloc(sizeof(char) * strlen(node));
    strcpy(node2, node);
    printf("node2 = %s\n", node2);
    
    list_node_t *item = list_find(list, node2);
    if(item == NULL)
        printf("cannot find item\n");
    else {
        list_remove(list, item);
        printf("after removal, list len = %d\n", list->len);
    }

    free(node2);
    list_destroy(list);

    return 0;
}

int main(void) {
    //ip_test();
    hashtable_test();
    return 0;
}
