#include "ft_nm.h"

void    close_fd(int *fd) {
    if (*fd > 0)
        close(*fd);
}

void    unmap_file(t_string *file) {
    if (file != NULL)
        munmap(file->content, file->size);
}

void    clear_map(t_btree **map) {
    map_clear(*map, free);
}

void    clear_list(t_list **list) {
    ft_list_clear(*list, NULL);
}

void    free_sym(void *ptr) {
    t_sym   *sym;

    sym = (t_sym*)ptr;
    free(sym->value);
    free(sym);
}