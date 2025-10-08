#include "ft_list.h"

void    ft_list_reverse(t_list **begin_list) {
    t_list  *new_list;
    t_list  *tmp;

    new_list = NULL;
    while (*begin_list != NULL) {
        ft_list_push_front(&new_list, (*begin_list)->data);
        tmp = (*begin_list)->next;
        free(*begin_list);
        *begin_list = tmp;
    }
    *begin_list = new_list;
}
