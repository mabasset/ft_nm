#include "ft_map.h"

static int  ft_strcmp(const char *s1, const char *s2) {
    int i;

    i = 0;
    while(s1[i] != '\0' && s1[i] == s2[i])
        i++;
    return s1[i] - s2[i];
}

int map_cmp_int(void *entry, void *key) {
    return *((int*)((t_map*)entry)->key) - *((int*)key);
}

int map_cmp_char(void *entry, void *key) {
    return *((char*)((t_map*)entry)->key) - *((char*)key);
}

int map_cmp_str(void *entry, void *key) {
    return ft_strcmp(((char*)((t_map*)entry)->key), (char*)key);
}

int map_cmp_entry_str(void *entry1, void *entry2) {
    return ft_strcmp(((char*)((t_map*)entry1)->key), ((char*)((t_map*)entry2)->key));
}