#include "ft_btree.h"

t_btree *btree_create_from_list(t_list *list, int (*cmpf)(void *, void *)) {
    t_btree *btree;

    btree = NULL;
    btree_insert_list(&btree, list, cmpf);

    return btree;
}