#include "ft_map.h"

void    map_insert_entries(t_btree **map, t_map *entries, int size, int (*cmp)(void *, void *)) {
    for(int i = 0; i < size; i++) {
        map_insert(map, entries[i].key, entries[i].value, cmp);
    }
}