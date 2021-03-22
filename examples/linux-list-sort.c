#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "list.h"
#include "list_sort.h"

static uint16_t values[10];

static inline int cmpu16(__attribute__((unused)) void *arrange,
                         struct list_head *a,
                         struct list_head *b)
{
    uint16_t ai = container_of(a, struct listitem, list)->i;
    uint16_t bi = container_of(b, struct listitem, list)->i;
    return ai - bi;
}

int main()
{
    struct list_head testlist;
    struct listitem *item, *is = NULL;
    size_t i;

    random_shuffle_array(values, (uint16_t) ARRAY_SIZE(values));
    INIT_LIST_HEAD(&testlist);

    for (i = 0; i < ARRAY_SIZE(values); i++) {
        item = (struct listitem *) malloc(sizeof(*item));
        assert(item);
        item->i = values[i];
        list_add_tail(&item->list, &testlist);
    }

    list_for_each_entry (item, &testlist, list) {
        printf("%d, ", item->i);
    }
    printf("\n");

    list_sort(NULL, &testlist, cmpu16);

    list_for_each_entry (item, &testlist, list) {
        printf("%d, ", item->i);
    }
    printf("\n");

    list_for_each_entry_safe (item, is, &testlist, list) {
        list_del(&item->list);
        free(item);
        i++;
    }

    return 0;
}
