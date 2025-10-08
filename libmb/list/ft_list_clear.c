#include "ft_list.h"

void    ft_list_clear(t_list *begin_list, void (*free_fct)(void *)) {
    t_list *tmp;

    while(begin_list != NULL) {
        tmp = begin_list->next;
        if (free_fct != NULL)
            free_fct(begin_list->data);
        free(begin_list);
        begin_list = tmp;
    }
}
