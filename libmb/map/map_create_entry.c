#include "ft_map.h"

t_map   *map_create_entry(const void *key, const void *value) {
    t_map   *entry;

    entry = (t_map *)malloc(sizeof(t_map));
    entry->key = key;
    entry->value = value;

    return entry;
}