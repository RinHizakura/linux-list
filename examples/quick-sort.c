#include <assert.h>
#include <stdlib.h>

#include "common.h"
#include "list.h"

static uint16_t values[256];

static void list_qsort(struct list_head *head)
{
    struct list_head list_less, list_greater;
    struct listitem *pivot;
    struct listitem *item = NULL, *is = NULL;

    if (list_empty(head) || list_is_singular(head))
        return;

    INIT_LIST_HEAD(&list_less);
    INIT_LIST_HEAD(&list_greater);

    pivot = list_first_entry(head, struct listitem, list);
    list_del(&pivot->list);

    list_for_each_entry_safe (item, is, head, list) {
        if (cmpint(&item->i, &pivot->i) < 0)
            list_move_tail(&item->list, &list_less);
        else
            list_move(&item->list, &list_greater);
    }

    list_qsort(&list_less);
    list_qsort(&list_greater);

    list_add(&pivot->list, head);
    list_splice(&list_less, head);
    list_splice_tail(&list_greater, head);
}

#define MAX_LEN 1000
static void list_qsort_no_recursive(struct list_head *head)
{
    struct listitem *begin[MAX_LEN], *end[MAX_LEN], *L, *R;
    struct listitem pivot;
    int i = 0;

    begin[0] = list_first_entry(head, struct listitem, list);
    end[0] = list_last_entry(head, struct listitem, list);

    while (i >= 0) {
        L = begin[i];
        R = end[i];

        if (L != R && &begin[i]->list != head) {
            // pivot is the actual address of L
            pivot = *begin[i];
            if (i == MAX_LEN - 1) {
                assert(-1);
                return;
            }

            while (L != R) {
                while (R->i >= pivot.i && L != R)
                    R = list_entry(R->list.prev, struct listitem, list);
                if (L != R) {
                    L->i = R->i;
                    L = list_entry(L->list.next, struct listitem, list);
                }

                while (L->i <= pivot.i && L != R)
                    L = list_entry(L->list.next, struct listitem, list);
                if (L != R) {
                    R->i = L->i;
                    R = list_entry(R->list.prev, struct listitem, list);
                }
            }
            L->i = pivot.i;
            begin[i + 1] = list_entry(L->list.next, struct listitem, list);
            end[i + 1] = end[i];
            end[i++] = L;
        } else
            i--;
    }
}

int main(void)
{
    struct list_head testlist;
    struct listitem *item, *is = NULL;
    size_t i;

    random_shuffle_array(values, (uint16_t) ARRAY_SIZE(values));

    INIT_LIST_HEAD(&testlist);

    assert(list_empty(&testlist));

    for (i = 0; i < ARRAY_SIZE(values); i++) {
        item = (struct listitem *) malloc(sizeof(*item));
        assert(item);
        item->i = values[i];
        list_add_tail(&item->list, &testlist);
    }

    assert(!list_empty(&testlist));

    qsort(values, ARRAY_SIZE(values), sizeof(values[0]), cmpint);
    list_qsort_no_recursive(&testlist);

    i = 0;
    list_for_each_entry_safe (item, is, &testlist, list) {
        assert(item->i == values[i]);
        list_del(&item->list);
        free(item);
        i++;
    }

    assert(i == ARRAY_SIZE(values));
    assert(list_empty(&testlist));

    return 0;
}
