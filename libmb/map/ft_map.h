#ifndef FT_MAP_H
# define FT_MAP_H

# include "../btree/ft_btree.h"

typedef struct {
    const void  *key;
    const void  *value;
} t_map;

t_map       *map_create_entry(const void *key, const void *value);
void        map_insert(t_btree **map, const void *key, const void *value, int (*cmp)(void *, void *));
void        map_insert_entries(t_btree **map, t_map *entries, int size, int (*cmp)(void *, void *));
t_btree     *map_create_from_entries(t_map *entries, int size, int (*cmp)(void *, void *));
void        map_apply(t_btree *map, void (*applyf)(void *));
void        map_clear(t_btree *map, void (*free_fct)(void *));
const void  *map_get(t_btree *map, void *key, int (*cmp)(void *, void *));

int     map_cmp_entry_str(void *entry1, void *entry2);
int     map_cmp_str(void *entry, void *key);

#endif