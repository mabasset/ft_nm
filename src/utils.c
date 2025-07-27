#include "ft_nm.h"

size_t    ft_strlen(char *str) {
    size_t i;

    i = 0;
    while (str[i] != '\0')
        i++;
    return i;
}

int ft_strcmp(char *s1, char *s2) {
    int i;

    for (i = 0; s1[i] != '\0'; i++) {
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

void    close_fd(int *fd) {
    if (*fd > 0)
        close(*fd);
}

void    unmap_file(t_string *mapped_file) {
    if (mapped_file->content != NULL)
        munmap(mapped_file->content, mapped_file->size);
}

void free_matrix(t_sym_info ***symbols_infos) {
    if (symbols_infos == NULL || *symbols_infos == NULL)
        return;
    for (int i = 0; (*symbols_infos)[i] != NULL; i++) {
        free((*symbols_infos)[i]->value);
        free((*symbols_infos)[i]);
    }
    free(*symbols_infos);
}

void    **ft_calloc(size_t size) {
    void    **ptr;

    ptr = malloc(sizeof(void *) * size);
    if (ptr == NULL)
        return NULL;
    for(size_t i = 0; i < size; i++)
        ptr[i] = NULL;
    return ptr;
}