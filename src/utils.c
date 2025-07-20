#include "ft_nm.h"

extern t_elf_file   g_elf_file;
extern t_flags      g_flags;

size_t    ft_strlen(char *str) {
    size_t i;

    i = 0;
    while (str[i] != '\0')
        i++;
    return i;
}

int ft_strcmp(char *s1, char *s2) {
    int i;

    for (i = 0; s1[i]; i++) {
        if (s1[i] > s2[i] || !s2[i])
            return 1;
        else if (s1[i] < s2[i])
            return -1;
    }
    if (s2[i])
        return -1;
    return 0;
}

char *ft_strchr(char *str, int search_str) {
    while (*str != '\0') {
        if (*str == search_str)
            return str;
        str++;
    }
    return NULL;
}
