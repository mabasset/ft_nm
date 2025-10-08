#include "ft_btree.h"

void    btree_insert_list(t_btree **root, t_list *list, int (*cmpf)(void *, void *)) {
    for(; list != NULL; list = list->next)
        btree_insert_data(root, list->data, cmpf);
}