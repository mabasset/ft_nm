#include "ft_map.h"

void    map_clear(t_btree *map, void (*free_fct)(void *)) {
    btree_clear(map, free_fct);
}