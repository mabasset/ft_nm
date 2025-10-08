#include "ft_list.h"

t_list  *ft_list_find(t_list *begin_list, void *data_ref, int (*cmp)(void *, void *)) {
    for(;begin_list != NULL; begin_list = begin_list->next)
        if (cmp(begin_list->data, data_ref) == 0)
            return begin_list;
    return NULL;
}
