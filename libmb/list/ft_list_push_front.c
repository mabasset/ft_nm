#include "ft_list.h"

void    ft_list_push_front(t_list **begin_list, void *data) {
    t_list  *node;

    node = ft_create_elem(data);
    if (*begin_list != NULL)
        node->next = *begin_list;
    *begin_list = node;
}
