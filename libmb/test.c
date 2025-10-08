#include "libmb.h"
#include <stdio.h>

void    map_print_str_str(void *ptr) {
    t_map *entry = (t_map*) ptr;

    printf("key: %s, value: %s\n", (char*)entry->key, (char*)entry->value);
}

int main() {
    t_btree *map;
    t_map   entries[] = {
        {"3", "v1"},
        {"1", "k2"},
        {"5", "k3"},
        {"2", "k4"},
        {"4", "k5"}
    };

    map = map_create_from_entries(entries, sizeof(entries) / sizeof(t_map), map_cmp_entry_str);
    const char *ciao = map_get(map, "4", map_cmp_str);
    ft_putstr(ciao);
    map_apply(map, map_print_str_str);
    map_clear(map, free);
}