#include "ft_map.h"

const void  *map_get(t_btree *map, void *key, int (*cmp)(void *, void *)) {
    t_map   *entry;

    entry = btree_sorted_search_item(map, key, cmp);
    if (entry == NULL)
        return NULL;

    return entry->value;
}