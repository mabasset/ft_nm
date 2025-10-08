#include "ft_map.h"

void    map_apply(t_btree *map, void (*applyf)(void *)) {
    btree_apply_inorder(map, applyf);
}