#include "ft_map.h"

t_btree *map_create_from_entries(t_map *entries, int size, int (*cmp)(void *, void *)) {
    t_btree *map;

    map = NULL;
    map_insert_entries(&map, entries, size, cmp);

    return map;
}