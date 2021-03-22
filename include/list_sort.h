#ifndef LIST_SORT_H
#define LIST_SORT_H

#include "list.h"

__attribute__((nonnull(2, 3))) void list_sort(void *priv,
                                              struct list_head *head,
                                              int (*cmp)(void *priv,
                                                         struct list_head *a,
                                                         struct list_head *b));
#endif
