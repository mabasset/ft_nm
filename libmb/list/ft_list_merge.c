#include "ft_list.h"

void    ft_list_merge(t_list **begin_list1, t_list *begin_list2) {
    t_list  *node;

    if (*begin_list1 != NULL) {
        node = ft_list_last(*begin_list1);
        node->next = begin_list2;
    }  else
        *begin_list1 = begin_list2;
}