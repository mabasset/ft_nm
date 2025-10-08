#include "ft_map.h"

void    map_insert(t_btree **map, const void *key, const void *value, int (*cmp)(void *, void *)) {
    t_map   *entry;

    entry = map_create_entry(key, value);
    if (btree_sorted_search_item(*map, entry, cmp) == NULL)
        btree_insert_data(map, entry, cmp);
}